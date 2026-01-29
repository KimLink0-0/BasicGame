// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentAction.h"
#include "Context.h"

class ILatentPollable
{
public:
	virtual ~ILatentPollable() = default;
	
	// 해당 함수가 true 를 반환하면 대기 끝, 재개
	virtual bool ShouldResume() = 0;
};

class FCoroLatentAction : public FPendingLatentAction
{
public:
	// 생성자 : 누가 (Poller) 기다리는지, 누구를 (Context) 깨워야 하는지
	FCoroLatentAction(ILatentPollable* InPoller, TSharedPtr<FCoroContext> InContext);
	
	// [필수] 매 프레임 엔진이 호출하는 함수
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
private:
	// 조건을 검사할 객체
	ILatentPollable* Poller;
	
	// 깨워야 할 코루틴의 Context
	TSharedPtr<FCoroContext> Context;
};
