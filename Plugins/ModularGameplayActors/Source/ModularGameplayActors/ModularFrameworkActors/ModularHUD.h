// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ModularHUD.generated.h"

UCLASS(Abstract)
class MODULARGAMEPLAYACTORS_API AModularHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AModularHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	virtual void BeginPlay() override;
};
