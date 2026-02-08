// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Condition/ExtensionCondition.h"
#include "Execute/ExtensionExecute.h"
#include "ActorExtension.generated.h"

USTRUCT(BlueprintType)
struct COMMONGAME_API FActorExtension
{
	GENERATED_BODY()
	
public:
	bool CanActivate(AActor* Owner) const;
	
	void OnActivate(AActor* Owner);
	
	void OnDeactivate(AActor* Owner);
	
	bool IsActivated() const { return bActivated; }
	
public:
	UPROPERTY()
	TArray<TInstancedStruct<FExtensionCondition>> Conditions;
	
	UPROPERTY()
	TArray<TInstancedStruct<FExtensionExecute>> Executes;
	
	
private:
	bool bActivated = false;
};
