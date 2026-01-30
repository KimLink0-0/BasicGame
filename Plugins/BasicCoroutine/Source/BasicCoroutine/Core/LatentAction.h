// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include "LatentActions.h"

namespace Coro::Private
{
	enum class ECoroLatentActionState : uint8
	{
		Running,		// 정상적으로 대기 중
		Completed,		// Resume 호출 후 종료
		Destroyed,		// OwnerActor 의 파괴 상태, Resume 없이 종료
	};
	
	class ILatentPollable
	{
	public:
		virtual ~ILatentPollable() = default;
		
		// 재개 시 true 를 반환하는 함수
		virtual bool ShouldResume() = 0;
		
		// 취소 시 정리 작업 진행
		virtual void OnCancel() = 0;
	};
	
	class BASICCOROUTINE_API FCoroLatentAction : public FPendingLatentAction
	{
	public:
		// 생성자
		FCoroLatentAction(ILatentPollable* InAwaiter, const FCoroContextPtr& InContext);
		virtual ~FCoroLatentAction() override;
		
		// FPendingLatentAction's Interface
		// 매 프레임 호출되는 함수, 이를 이용해서 Awaiting 구현
		virtual void UpdateOperation(FLatentResponse& Response) override;
		
		// 액션이 중단될 때 호출
		virtual void NotifyActionAborted() override;
		
		// Owner 오브젝트가 파괴될 때 호출
		virtual void NotifyObjectDestroyed() override;
		
	private:
		ECoroLatentActionState State = ECoroLatentActionState::Running;
		
		ILatentPollable* Awaiter;
		
		FCoroContextPtr Context;
	};
	
}