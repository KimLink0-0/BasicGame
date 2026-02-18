// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Ability/CommonAbilityTypes.h"
#include "CommonAbilitySystemComponent.generated.h"


UCLASS()
class COMMONGAME_API UCommonAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCommonAbilitySystemComponent(const FObjectInitializer& InitializerModule = FObjectInitializer::Get());
	
	// Notify 설정 
	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	
	virtual void PostNetInit();
	
	bool IsPostNetInitialized() const { return bPostNetInitialized;};
	
	
protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	int32 ActivationGroupCounts[static_cast<uint8>(ECommonAbilityActivationGroup::MAX)];
	
	bool bPostNetInitialized = false;
};
