// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Coroutine.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularGameModeBase.h"
#include "CommonGameModeBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(CommonGameModeLog, Log, All);

UCLASS()
class COMMONGAME_API ACommonGameModeBase : public AModularGameModeBase
{
	GENERATED_BODY()
	
public:
	ACommonGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
private:
	
	TCoroTask<void> StartExperienceLoadCoroutine();
};
