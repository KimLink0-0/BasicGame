// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Coroutine.h" // 코루틴을 사용하기 위해서 헤더파일 추가
#include "GameFramework/Actor.h"
#include "CoroutineTestActor.generated.h"

UCLASS()
class BASICGAME_API ACoroutineTestActor : public AActor
{
	GENERATED_BODY()

public:
	ACoroutineTestActor();
	
	// 테스트할 코루틴 함수 생성
	TCoroTask<void> FirstTestCoroutine();

protected:
	virtual void BeginPlay() override;

};
