// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputTypes.generated.h"


class UInputAction;

USTRUCT(BlueprintType)
struct COMMONGAME_API FInputActionAndTag
{
	GENERATED_BODY()
	
public:
	bool IsValid() const
	{
		return InputTag.IsValid() && !InputAction.IsNull();
	}
	
	FString ToString() const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FGameplayTag InputTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AssetBundle = "Client"))
	TSoftObjectPtr<UInputAction> InputAction;
};
