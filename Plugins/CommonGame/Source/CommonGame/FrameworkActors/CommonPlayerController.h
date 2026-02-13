// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularPlayerController.h"
#include "CommonPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPostProcessInput, float /*DeltaTime*/, bool /*bGamePaused*/);

UCLASS()
class COMMONGAME_API ACommonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ACommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void ReceivedPlayer() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	
public:
	FOnPostProcessInput OnPostProcessInput;
};
