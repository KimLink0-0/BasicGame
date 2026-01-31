// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Coroutine.h"
#include "GameFramework/Actor.h"
#include "CoroutineTestActor.generated.h"

UCLASS()
class BASICGAME_API ACoroutineTestActor : public AActor
{
	GENERATED_BODY()

public:
	ACoroutineTestActor();
	
	TCoroTask<void> TimeCoroutineTestFunction(UObject* InOwner);

protected:
	virtual void BeginPlay() override;

};
