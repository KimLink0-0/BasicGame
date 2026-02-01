// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

UCLASS(Abstract)
class MODULARGAMEPLAYACTORS_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AModularCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	virtual void BeginPlay() override;
};
