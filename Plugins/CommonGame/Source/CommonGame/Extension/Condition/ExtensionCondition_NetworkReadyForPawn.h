// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtensionCondition.h"
#include "ExtensionCondition_NetworkReadyForPawn.generated.h"

class UAttributeSet;

USTRUCT(BlueprintType, meta = (DisplayName = "Network Ready"))
struct COMMONGAME_API FExtensionCondition_NetworkReadyForPawn : public FExtensionCondition
{
	GENERATED_BODY()
public:
	virtual bool IsSatisfied(AActor* Owner) const override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Condition")
	uint8 bRequirePlayerController : 1 = true;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	uint8 bRequirePlayerStateLinked : 1 = true;
	
	UPROPERTY(EditAnywhere, Category = "Condition")
	uint8 bRequireAbilitySystemReady : 1 = true;
	
	// UPROPERTY(EditAnywhere, Category = "Condition", meta = (EditCondition = "bRequireAbilitySystemReady"))
	// TArray<TSubclassOf<UAttributeSet>> RequiredAttributeSets;
};
