// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "ModularDefaultPawn.generated.h"

UCLASS(Abstract)
class MODULARGAMEPLAYACTORS_API AModularDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	AModularDefaultPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	virtual void BeginPlay() override;
};
