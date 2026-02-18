#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonAbilityTypes.generated.h"

class UCommonGameplayAbility;

USTRUCT(BlueprintType)
struct COMMONGAME_API FCommonAbilityEntry
{
	GENERATED_BODY()
	
public:
	// 에디터에서 맵핑하는 값
	
	// 어떤 어빌리티를 발동할지
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonGameplayAbility> Ability;
	
	// 어떤 InputTag 와 맵핑되어 발동할지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag;
};

// Input 측면에서 어떤 식으로 활성화 될 지
UENUM(BlueprintType)
enum class ECommonAbilityActivationPolicy : uint8
{
	// 입력 시작 시 활성화
	OnInputTriggered,
	
	// 입력 유지 중 활성화
	WhileInputActive,
	
	// 스폰 시 활성화
	OnSpawn
};

// 사용자 경험 측면에서 어떤 식으로 활성화 될 지 
UENUM(BlueprintType)
enum class ECommonAbilityActivationGroup : uint8
{
	// 항상 활성화 가능한 어빌리티
	Independent,
	
	// 새로운 Exclusive 시전 시 취소됨
	Exclusive_Replaceable,
	
	// 활성화 중 Exclusive 진입 차단
	Exclusive_Blocking,
	
	// 카운트를 위한 것 사용하지 않음
	MAX UMETA(Hidden)
};
