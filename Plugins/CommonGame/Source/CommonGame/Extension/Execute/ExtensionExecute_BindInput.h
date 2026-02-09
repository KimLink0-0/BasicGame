// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtensionExecute.h"
#include "CommonGame/Input/InputTypes.h"
#include "ExtensionExecute_BindInput.generated.h"


struct FInputActionValue;
class UInputMappingContext;

USTRUCT(BlueprintType, meta = (DisplayName = "Bind Input"))
struct COMMONGAME_API FExtensionExecute_BindInput : public FExtensionExecute
{
	GENERATED_BODY()
	
public:
	virtual void OnActivate(AActor* Owner) const override;
	virtual void OnDeactivate(AActor* Owner) const override;
	
private:
	// ------------------- Native Action --------------
	void Input_Move(const FInputActionValue& InputActionValue) const;
	
	void Input_Look(const FInputActionValue& InputActionValue) const;
	
	// ------------------- Ability 처리 ----------------
	
	
public:
	UPROPERTY(EditAnywhere, Category = "Mapping Context", meta = (AssetBundles = "Client"))
	TSoftObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Mapping Context")
	int32 Priority = 0;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions", meta = (TitleProperty = "{InputTag}"))
	TArray<FInputActionAndTag> NativeInputActions;
	
	UPROPERTY(EditAnywhere, Category = "Input Actions", meta = (TitleProperty = "{InputTag}"))
	TArray<FInputActionAndTag> AbilityInputActions;
	
private:
	// 활성화 된 Pawn 잠시 캐싱해서 사용
	mutable TWeakObjectPtr<APawn> WeakPawn;
	
	// EnhancedInputComponent 에서 바인딩 할 때 반환하는 Handle 을 보관
	mutable TArray<uint32> BindingHandles; 
};
