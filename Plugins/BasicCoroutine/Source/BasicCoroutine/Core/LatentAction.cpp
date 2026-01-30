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
		
		// 재개 조건 확인: Awaiter 의 ShouldResume 가 true 면 
		// Awaiter 초기화, State 를 Completed 로 변환 후 Resume() 프로세스로 넘어감 
		if (State == ECoroLatentActionState::Running && Awaiter->ShouldResume())
		{
			Awaiter = nullptr;
			State = ECoroLatentActionState::Completed;
		}
		
		// Resume() 프로세스: 대기가 끝났으면, Resume() 후 종료
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

