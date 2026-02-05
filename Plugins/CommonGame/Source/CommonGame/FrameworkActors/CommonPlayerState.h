// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularPlayerState.h"
#include "CommonPlayerState.generated.h"

UCLASS()
class COMMONGAME_API ACommonPlayerState : public AModularPlayerState
{
	GENERATED_BODY()
	
public:
	ACommonPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
