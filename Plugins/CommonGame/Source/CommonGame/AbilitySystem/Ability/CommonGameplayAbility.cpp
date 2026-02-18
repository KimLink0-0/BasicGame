// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameplayAbility.h"
#include "CommonGame/AbilitySystem/CommonAbilitySystemComponent.h"

UCommonGameplayAbility::UCommonGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
	// Ability 자체는 복제하지 않게 설정
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	
	// 소유자당 1개의 인스턴스 사용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// 클라이언트 예측 사용
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	// 클라이언트/서버 모두 사용 사용 가능
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

bool UCommonGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// 어빌리티
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}
	
	// 수퍼 함수가 실행 불가능하게 하면 실행 불가능
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags))
	{
		return false;
	}
	
	// 내가 설정한 AbilityComponent 가 있어야 활성화 정책과 그룹이 유의미 하기 때문에 캐스팅 가능한지 체크
	const UCommonAbilitySystemComponent* CommonASC = Cast<UCommonAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	// ASC 설계 완료 후 Activation 가능 조건 체크하는 로직을 if 문 안에 혹은 별도의 if 문으로 추가 검사
	if (CommonASC)
	{
		return false;
	}
	
	return true;
}

void UCommonGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (ActivationPolicy == ECommonAbilityActivationPolicy::OnSpawn)
	{
		if (UCommonAbilitySystemComponent* ASC = Cast<UCommonAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()))
		{
			ASC->TryActivateAbility(Spec.Handle);
		}
	}
}

void UCommonGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}
