#include "Context.h"
#include "Promise.h"

namespace Coro::Private
{
	bool FCoroContext::IsDone() const
	{
		// std::memory_order_acquire : State 값을 먼저 읽어오고, 아래 코드를 실행하라
		ECoroState Current = State.load(std::memory_order_acquire);
		return Current == ECoroState::Completed || Current == ECoroState::Canceled;
	}

	bool FCoroContext::IsCancelRequested() const
	{
		return bCancelRequested.load(std::memory_order_acquire);
	}

	void FCoroContext::MarkCompleted()
	{
		{
			FScopeLock ScopeLock(&Lock);
			
			// std::memory_order_release : 이전에 하던 작업이 전부 다 끝나면 Completed 로 바꿔 쓰는 것
			State.store(ECoroState::Completed, std::memory_order_release);
			
			Promise = nullptr;
		}
	}

	void FCoroContext::AddCompletionCallback(TFunction<void()> Callback)
	{
		bool bShouldInvokeImmediately = false;
		
		{
			FScopeLock ScopeLock(&Lock);
			
			if (IsDone())
			{
				bShouldInvokeImmediately = true;
			}
			else
			{
				CompletionCallbacks.Add(MoveTemp(Callback));
			}
		}
		
		if (bShouldInvokeImmediately)
		{
			Callback();
		}
	}

	void FCoroContext::Cancel()
	{
		TArray<TFunction<void()>> CallbacksToInvoke;
		
		{
			FScopeLock ScopeLock(&Lock);
			
			// 이미 취소 요청이 됐으면 넘어감
			if (bCancelRequested.load(std::memory_order_acquire))
			{
				return;
			}
			
			bCancelRequested.store(true, std::memory_order_release);
			
			CallbacksToInvoke = MoveTemp(CancellationCallbacks);
		}
		
		for (const TFunction<void()>& Callback : CallbacksToInvoke)
		{
			Callback();
		}
	}

	void FCoroContext::Resume() const
	{
		if (Promise)
		{
			Promise->Resume();
		}
	}

	void FCoroContext::Destroy()
	{
		if (IsDone())
		{
			return;
		}
		
		if (Promise)
		{
			Promise->DestroyCoroutine();
		}
		
		bCancelRequested.store(true, std::memory_order_release);
		State.store(ECoroState::Canceled, std::memory_order_release);
	}

	void FCoroContext::SetPromise(FPromise* InPromise)
	{
		Promise = InPromise;
	}
}