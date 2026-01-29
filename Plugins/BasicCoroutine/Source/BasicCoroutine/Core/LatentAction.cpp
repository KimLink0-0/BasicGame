// Fill out your copyright notice in the Description page of Project Settings.


#include "LatentAction.h"


FCoroLatentAction::FCoroLatentAction(ILatentPollable* InPoller, TSharedPtr<FCoroContext> InContext) :
	Poller(InPoller),
	Context(InContext)
{
}

void FCoroLatentAction::UpdateOperation(FLatentResponse& Response)
{
	// 1. 코루틴이 종료 되었는지 확인
	if (Context->IsDone())
	{
		Response.DoneIf(true);
		return;
	}
	
	// 2. 조건이 만족됐는지 확인 (예: 대기 시간이 지났는가?)
	bool bConditionMet = Poller->ShouldResume();
	
	// 3. 조건 만족 시 코루틴 재개
	if (bConditionMet)
	{
		Context->Resume();
	}
	
	Response.DoneIf(bConditionMet);
}
