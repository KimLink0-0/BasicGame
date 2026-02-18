// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "CommonGameplayAbility.generated.h"

UCLASS(Abstract)
class COMMONGAME_API UCommonGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UCommonGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// AbilitySystemComponent 에서 활용할 Getter 
	ECommonAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ECommonAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }
	
	
protected:
	// Ability 를 설정하기 위한 3대장
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	ECommonAbilityActivationPolicy ActivationPolicy = ECommonAbilityActivationPolicy::OnInputTriggered;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	ECommonAbilityActivationGroup ActivationGroup = ECommonAbilityActivationGroup::Independent;
};
