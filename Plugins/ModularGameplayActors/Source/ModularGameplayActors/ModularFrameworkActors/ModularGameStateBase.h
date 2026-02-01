// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ModularGameStateBase.generated.h"

UCLASS(Abstract)
class MODULARGAMEPLAYACTORS_API AModularGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AModularGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	virtual void BeginPlay() override;
};
