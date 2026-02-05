// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularHUD.h"
#include "CommonHUD.generated.h"

UCLASS()
class COMMONGAME_API ACommonHUD : public AModularHUD
{
	GENERATED_BODY()
public:
	ACommonHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
