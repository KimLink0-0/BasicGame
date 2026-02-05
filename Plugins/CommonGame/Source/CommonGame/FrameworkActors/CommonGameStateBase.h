// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularGameStateBase.h"
#include "CommonGameStateBase.generated.h"

class UExperienceManagerComponent;

UCLASS()
class COMMONGAME_API ACommonGameStateBase : public AModularGameStateBase
{
	GENERATED_BODY()
public:
	ACommonGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UExperienceManagerComponent* GetExperienceManagerComponent() const { return ExperienceManagerComponent; }
	
private:
	UPROPERTY()
	TObjectPtr<UExperienceManagerComponent> ExperienceManagerComponent;
};
