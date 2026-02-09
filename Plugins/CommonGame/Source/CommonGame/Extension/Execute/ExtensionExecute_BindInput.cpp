// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtensionExecute_BindInput.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "CommonGame/CommonGameplayTags.h"
#include "CommonGame/Input/CommonEnhancedInputComponent.h"

void FExtensionExecute_BindInput::OnActivate(AActor* Owner) const
{
	// Pawn 전용 Execute
	APawn* Pawn = Cast<APawn>(Owner);
	APlayerController* PC = Pawn ? Pawn->GetController<APlayerController>() : nullptr;
	if (!PC || !PC->GetLocalPlayer())
	{
		return;
	}

	WeakPawn = Pawn;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	UCommonEnhancedInputComponent* InputComponent = Cast<UCommonEnhancedInputComponent>(Pawn->InputComponent);

	if (!Subsystem || !InputComponent)
	{
		return;
	}

	// MappingContext 등록
	if (UInputMappingContext* LoadedContext = MappingContext.Get())
	{
		Subsystem->AddMappingContext(LoadedContext, Priority);
	}

	// Input 매핑 설정
	InputComponent->SetNativeInputActionMappings(NativeInputActions);
	InputComponent->SetAbilityInputActionMappings(AbilityInputActions);

	// Move 입력 바인딩 (람다 사용)
	if (uint32 MoveHandle = InputComponent->BindNativeActionValueLambda(
		CommonGameplayTags::Input_Native_Move,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) { Input_Move(Value); }))
	{
		BindingHandles.Add(MoveHandle);
	}

	// Look 입력 바인딩 (람다 사용)
	if (uint32 LookHandle = InputComponent->BindNativeActionValueLambda(
		CommonGameplayTags::Input_Native_Look,
		ETriggerEvent::Triggered,
		[this](const FInputActionValue& Value) { Input_Look(Value); }))
	{
		BindingHandles.Add(LookHandle);
	}

	// // Ability 입력 바인딩 - ASC로 라우팅
	// for (const FInputActionAndTag& Mapping : AbilityInputActions)
	// {
	// 	if (!Mapping.InputTag.IsValid())
	// 	{
	// 		continue;
	// 	}
	//
	// 	const FGameplayTag InputTag = Mapping.InputTag;
	//
	// 	FAbilityInputBindingHandles Handles = InputComponent->BindAbilityActionLambda(
	// 		InputTag,
	// 		[this, InputTag](const FInputActionInstance&) { Input_AbilityPressed(InputTag); },
	// 		[this, InputTag](const FInputActionInstance&) { Input_AbilityReleased(InputTag); }
	// 	);
	//
	// 	if (Handles.IsValid())
	// 	{
	// 		BindingHandles.Add(Handles.PressHandle);
	// 		BindingHandles.Add(Handles.ReleaseHandle);
	// 	}
	// }
}

void FExtensionExecute_BindInput::OnDeactivate(AActor* Owner) const
{
	FExtensionExecute::OnDeactivate(Owner);
}

void FExtensionExecute_BindInput::Input_Move(const FInputActionValue& InputActionValue) const
{
	APawn* Pawn = WeakPawn.Get();
	if (!Pawn)
	{
		return;
	}
	
	AController* Controller = Pawn->GetController();
	if (!Controller)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		Pawn->AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		Pawn->AddMovementInput(MovementDirection, Value.Y);
	}
}

void FExtensionExecute_BindInput::Input_Look(const FInputActionValue& InputActionValue) const
{
	APawn* Pawn = WeakPawn.Get();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}
