// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Awaiter.h"

namespace Coro::Private
{
	class BASICCOROUTINE_API FWhenAllCompletedAwaiter : public TAsyncAwaiterBase<FWhenAllCompletedAwaiter>
	{
		using Super = TAsyncAwaiterBase<FWhenAllCompletedAwaiter>;
		
	public:
		FWhenAllCompletedAwaiter(UObject* InOwner, TArray<TCoroTask<void>> InTasks);
		
		// 모든 Task 가 준비된 상태라면 suspend 건너 뜀
		bool Ready() const;
		
		// 모든 Task 가 완료됐다면 재개
		void Suspend();
		
	private:
		// 대기 중인 Task 목록
		TArray<TCoroTask<void>> Tasks;
		
		// 남은 Task 수
		std::atomic<int32> RemainingCount{0};
	};
	
	
	class BASICCOROUTINE_API FWhenAnyCompletedAwaiter : public TAsyncAwaiterBase<FWhenAnyCompletedAwaiter>
	{
		using Super = TAsyncAwaiterBase<FWhenAnyCompletedAwaiter>;
		
	public:
		FWhenAnyCompletedAwaiter(UObject* InOwner, TArray<TCoroTask<void>> InTasks);
		
		// 이미 완료된 Task 가 존재하면 suspend 건너 뜀
		bool Ready();
		
		// Task 가 하나라도 완료 시 재개
		void Suspend();
		
		// 완료된 Task 의 Index 를 반환
		int32 GetResult() const;
		
	private:
		// 대기 중인 Task 목록
		TArray<TCoroTask<void>> Tasks;
		
		// 이미 재개되었는지 확인용
		std::atomic<bool> bResumed{false};
		
		// 완료된 Task 수
		int32 CompletedIndex = INDEX_NONE;
	};
}

namespace Coro::Async
{
	inline Private::FWhenAllCompletedAwaiter WhenAllCompleted(UObject* InOwner, TArray<TCoroTask<void>> InTasks)
	{
		return Private::FWhenAllCompletedAwaiter(InOwner, MoveTemp(InTasks));
	}
	
	inline Private::FWhenAnyCompletedAwaiter WhenAnyCompleted(UObject* InOwner, TArray<TCoroTask<void>> InTasks)
	{
		return Private::FWhenAnyCompletedAwaiter(InOwner, MoveTemp(InTasks));
	}
}
