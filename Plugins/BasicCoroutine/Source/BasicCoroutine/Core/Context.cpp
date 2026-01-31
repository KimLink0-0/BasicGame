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
		
	void FCoroContext::MarkCompleted()
	{
		{
			FScopeLock ScopeLock(&Lock);
			
			// std::memory_order_release : 이전에 하던 작업이 전부 다 끝나면 Completed 로 바꿔 쓰는 것
			State.store(ECoroState::Completed, std::memory_order_release);
			
			Promise = nullptr;
		}
	}
		
	void FCoroContext::Resume() const
	{
		if (Promise)
		{
			Promise->Resume();
		}
	}
		
	void FCoroContext::SetPromise(FPromise* InPromise)
	{
		Promise = InPromise;
	}
}