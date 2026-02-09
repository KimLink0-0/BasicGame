// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputTypes.h"
#include "CommonEnhancedInputComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(CommonEnhancedInputComponentLog, Log, All);

struct FAbilityInputBindingHandles
{
public:
	bool IsValid() const { return PressHandle != 0 && ReleaseHandle != 0; }
	
public:
	uint32 PressHandle = 0;
	
	uint32 ReleaseHandle = 0;
};


UCLASS()
class COMMONGAME_API UCommonEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	void SetNativeInputActionMappings(const TArray<FInputActionAndTag>& InMappings);
	
	void SetAbilityInputActionMappings(const TArray<FInputActionAndTag>& InMappings);
	
	// BindingAction Templates 
	// 태그로 찾아서 바인딩
	// --------------------------------  Bind Native Action ----------------------------------  
#pragma region NativeInputBindings
	
	
	// 파라미터가 없는 콜백 바인딩 : 점프, 재장전 등 토글 방식의 동작에 사용
	template<class UserClass, typename... VarTypes>
	uint32 BindNativeAction(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, TMemFunPtrType<false, UserClass, void(VarTypes)>::Type Func, VarTypes...Vars)
	{
		if (const UInputAction* Action = FindNativeActionByTag(InputTag))
		{
			return BindAction(Action, TriggerEvent, Object, Func, Vars...).GetHandle();
		}
		
		UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindNativeAction: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		return 0;
	}
	
	// 파라미터가 존재하는 콜백 바인딩 : (WASD 이동 FVector2D), (시점 이동 -> FVector2D) 등 
	template<class UserClass, typename... VarTypes>
	uint32 BindNativeAction(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, TMemFunPtrType<false, UserClass, void(const FInputActionValue&, VarTypes...)>::Type Func, VarTypes...Vars)
	{
		if (const UInputAction* Action = FindNativeActionByTag(InputTag))
		{
			return BindAction(Action, TriggerEvent, Object, Func, Vars...).GetHandle();
		}
		
		UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindNativeAction: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		return 0;
	}
	
	// 파라미터와 함께 메타데이터가 필요한 콜백 바인딩 : (차징 공격, 누른 시간에 따라 위력 변화), (홀드 입력 상호작용)
	template<class UserClass, typename... VarTypes>
	uint32 BindNativeAction(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, TMemFunPtrType<false, UserClass, void(const FInputActionInstance&, VarTypes...)>::Type Func, VarTypes...Vars)
	{
		if (const UInputAction* Action = FindNativeActionByTag(InputTag))
		{
			return BindAction(Action, TriggerEvent, Object, Func, Vars...).GetHandle();
		}
		
		UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindNativeAction: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		return 0;
	}
	
	// 람다 콜백 바인딩
	template<typename FuncType>
	uint32 BindNativeActionValueLambda(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, FuncType&& Func)
	{
		if (const UInputAction* Action = FindNativeActionByTag(InputTag))
		{
			return BindActionValueLambda(Action, TriggerEvent, Forward<FuncType>(Func)).GetHandle();
		}

		UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindNativeActionValueLambda: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		return 0;
	}
	
	// 메타데이터가 필요한 람다 콜백 바인딩 
	template<typename FuncType>
	uint32 BindNativeActionInstanceLambda(const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, FuncType&& Func)
	{
		if (const UInputAction* Action = FindNativeActionByTag(InputTag))
		{
			return BindActionInstanceLambda(Action, TriggerEvent, Forward<FuncType>(Func)).GetHandle();
		}

		UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindNativeActionInstanceLambda: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		return 0;
	}
#pragma endregion
	
	// --------------------------------  Bind Ability Action ----------------------------------
#pragma region AbilityInputBindings
	
	template<class UserClass>
	FAbilityInputBindingHandles BindAbilityAction(const FGameplayTag& InputTag, UserClass* Object, TMemFunPtrType<false, UserClass, void(FGameplayTag, bool)>::Type Func)
	{
		FAbilityInputBindingHandles Handles;

		if (const UInputAction* Action = FindAbilityActionByTag(InputTag))
		{
			// Press (Started)
			Handles.PressHandle = BindAction(Action, ETriggerEvent::Started, Object, Func, InputTag, true).GetHandle();

			// Release (Completed)
			Handles.ReleaseHandle = BindAction(Action, ETriggerEvent::Completed, Object, Func, InputTag, false).GetHandle();
		}
		else
		{
			UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindAbilityAction: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		}

		return Handles;
	}
	
	// 람다 바인딩은 UObject없이 람다 함수를 바인딩하기 위해서 사용 
	template<typename PressFuncType, typename ReleaseFuncType>
	FAbilityInputBindingHandles BindAbilityActionLambda(const FGameplayTag& InputTag, PressFuncType&& PressFunc, ReleaseFuncType&& ReleaseFunc)
	{
		FAbilityInputBindingHandles Handles;

		if (const UInputAction* Action = FindAbilityActionByTag(InputTag))
		{
			// Press (Started)
			Handles.PressHandle = BindActionInstanceLambda(Action, ETriggerEvent::Started, Forward<PressFuncType>(PressFunc)).GetHandle();

			// Release (Completed)
			Handles.ReleaseHandle = BindActionInstanceLambda(Action, ETriggerEvent::Completed, Forward<ReleaseFuncType>(ReleaseFunc)).GetHandle();
		}
		else
		{
			UE_LOG(CommonEnhancedInputComponentLog, Warning, TEXT("BindAbilityActionLambda: [%s] 태그에 해당하는 InputAction을 찾을 수 없습니다"), *InputTag.ToString());
		}

		return Handles;
	}
#pragma endregion
	
	
private:
	const UInputAction* FindNativeActionByTag(const FGameplayTag& InputTag) const;
	
	const UInputAction* FindAbilityActionByTag(const FGameplayTag& InputTag) const;
	
	
	
private:
	TArray<FInputActionAndTag> NativeInputActionMappings;
	
	TArray<FInputActionAndTag> AbilityInputActionMappings;
};
