// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModularGameModeBase.generated.h"

UCLASS(Abstract)
class MODULARGAMEPLAYACTORS_API AModularGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AModularGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
		
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	virtual void BeginPlay() override;
};
