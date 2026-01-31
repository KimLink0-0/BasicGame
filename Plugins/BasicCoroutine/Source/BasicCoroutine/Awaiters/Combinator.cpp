// Fill out your copyright notice in the Description page of Project Settings.


#include "Combinator.h"

namespace Coro::Private
{
	
	FWhenAllCompletedAwaiter::FWhenAllCompletedAwaiter(UObject* InOwner, TArray<TCoroTask<void>> InTasks) :
		Super(InOwner),
		Tasks(MoveTemp(InTasks))
	{
	}

	bool FWhenAllCompletedAwaiter::Ready() const
	{
		for (const auto& Task : Tasks)
		{
			// Task 가 유효한지 확인
			if (!Task.IsValid())
			{
				return false;
			}
		}
		return false;
	}

	void FWhenAllCompletedAwaiter::Suspend()
	{
		RemainingCount.store(Tasks.Num());
		
		for (auto& Task : Tasks)
		{
			Task.ContinueWith([CapturedOwner = Owner, CapturedContext = Context, Counter = &RemainingCount]()
			{
				// Counter 의 메모리에서 값을 가져오고 1을 빼기를 할 건데, 
				// 1을 빼기 직전의 값이 1이라면 재개
				if (Counter->fetch_sub(1) == 1)
				{
					SafeResume(CapturedOwner, CapturedContext);
				}
			});
		}
	}

	FWhenAnyCompletedAwaiter::FWhenAnyCompletedAwaiter(UObject* InOwner, TArray<TCoroTask<void>> InTasks) :
		Super(InOwner),
		Tasks(MoveTemp(InTasks))
	{
	}

	bool FWhenAnyCompletedAwaiter::Ready() 
	{
		for (int32 i = 0; i < Tasks.Num(); ++i)
		{
			if (Tasks[i].IsDone())
			{
				CompletedIndex = i;
				return true;
			}	
		}
		return false;
	}

	void FWhenAnyCompletedAwaiter::Suspend()
	{
		bResumed.store(false);
		
		for (int32 i = 0; i < Tasks.Num(); ++i)
		{
			Tasks[i].ContinueWith([CapturedOwner = Owner, CapturedContext = Context, Resumed = &bResumed, Index = i, ResultIndex = &CompletedIndex]()
			{
				bool bExpected = false;
				if (Resumed->compare_exchange_strong(bExpected, true))
				{
					*ResultIndex = Index;
					SafeResume(CapturedOwner, CapturedContext);
				}
			});
		}
	}

	int32 FWhenAnyCompletedAwaiter::GetResult() const
	{
		return CompletedIndex;
	}

}