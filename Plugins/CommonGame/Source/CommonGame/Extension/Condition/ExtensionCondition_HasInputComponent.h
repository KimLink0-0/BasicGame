// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtensionCondition.h"
#include "ExtensionCondition_HasInputComponent.generated.h"


USTRUCT(BlueprintType, meta = (DisplayName = "Has InputComponent"))
struct COMMONGAME_API FExtensionCondition_HasInputComponent : public FExtensionCondition
{
	GENERATED_BODY()
public:
	virtual bool IsSatisfied(AActor* Owner) const override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Condition", meta = (AllowAbstract = "false"))
	TSubclassOf<UInputComponent> RequiredClass;
};
