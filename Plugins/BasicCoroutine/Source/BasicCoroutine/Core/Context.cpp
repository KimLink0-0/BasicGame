#include "Context.h"
#include "Promise.h"

namespace Coro::Private
{
	bool FCoroContext::IsDone() const
	{
		ECoroState Current = State.load(std::memory_order_relaxed);
		return Current == ECoroState::Completed || Current == ECoroState::Canceled;
	}
		
	void FCoroContext::MarkCompleted()
	{
		State.store(ECoroState::Completed, std::memory_order_relaxed);
	}
		
	void FCoroContext::Cancel()
	{
		ECoroState Expected = ECoroState::Pending;
		if (!State.compare_exchange_strong(Expected, ECoroState::Canceled))
		{
			Expected = ECoroState::Running;
			State.compare_exchange_strong(Expected, ECoroState::Canceled);
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