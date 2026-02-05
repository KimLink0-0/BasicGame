// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularCharacter.h"
#include "CommonCharacter.generated.h"

UCLASS()
class COMMONGAME_API ACommonCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	ACommonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
