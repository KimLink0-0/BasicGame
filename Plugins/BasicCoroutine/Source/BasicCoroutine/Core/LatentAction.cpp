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
		// 만약 오너가 파괴되거나 액션이 종료되면, 재개 없이 종료
		if (State == ECoroLatentActionState::Destroyed)
		{
			Response.DoneIf(true);
			return;
		}
		
		// 대기가 끝났으면, Resume() 후 종료
		if (State == ECoroLatentActionState::Completed)
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

