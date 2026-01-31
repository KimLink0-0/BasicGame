// Fill out your copyright notice in the Description page of Project Settings.


#include "LatentAction.h"
#include "Context.h"

namespace Coro::Private
{
	FCoroLatentAction::FCoroLatentAction(ILatentPollable* InAwaiter, const FCoroContextPtr& InContext) :
		Awaiter(InAwaiter),	
		Context(InContext)
	{
		
	}
	
	FCoroLatentAction::~FCoroLatentAction()
	{
	}

	void FCoroLatentAction::UpdateOperation(FLatentResponse& Response)
	{
		// Owner가 파괴된 경우 즉시 종료 (Resume 호출 안함)
		if (State == ECoroLatentActionState::Destroyed)
		{
			Response.DoneIf(true);
			return;
		}

		// Awaiter가 없으면 대기할 것이 없으므로 즉시 종료
		if (State == ECoroLatentActionState::Running && !Awaiter)
		{
			Response.DoneIf(true);
			return;
		}

		// 재개 조건 확인
		if (State == ECoroLatentActionState::Running && Awaiter->ShouldResume())
		{
			Awaiter = nullptr;
			State = ECoroLatentActionState::Completing;
		}

		// 완료 대기 상태면 Resume 호출 후 종료
		if (State == ECoroLatentActionState::Completing)
		{
			Context->Resume();
			Context = nullptr;
			Response.DoneIf(true);
		}
	}

	void FCoroLatentAction::NotifyActionAborted()
	{
		State = ECoroLatentActionState::Destroyed;
	}

	void FCoroLatentAction::NotifyObjectDestroyed()
	{
		State = ECoroLatentActionState::Destroyed;
	}
}

