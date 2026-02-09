// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonEnhancedInputComponent.h"

void UCommonEnhancedInputComponent::SetNativeInputActionMappings(const TArray<FInputActionAndTag>& InMappings)
{
	NativeInputActionMappings = InMappings;
}

void UCommonEnhancedInputComponent::SetAbilityInputActionMappings(const TArray<FInputActionAndTag>& InMappings)
{
	AbilityInputActionMappings = InMappings;
}

const UInputAction* UCommonEnhancedInputComponent::FindNativeActionByTag(const FGameplayTag& InputTag) const
{
	for (const FInputActionAndTag& Mapping : NativeInputActionMappings)
	{
		if (Mapping.InputTag.MatchesTagExact(InputTag))
		{
			return Mapping.InputAction.Get();
		}
	}
	
	return nullptr;
}

const UInputAction* UCommonEnhancedInputComponent::FindAbilityActionByTag(const FGameplayTag& InputTag) const
{
	for (const FInputActionAndTag& Mapping : AbilityInputActionMappings)
	{
		if (Mapping.InputTag.MatchesTagExact(InputTag))
		{
			return Mapping.InputAction.Get();
		}
	}
	
	return nullptr;
}
