// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPlayerController.h"

ACommonPlayerController::ACommonPlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
}

void ACommonPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void ACommonPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);
	
	OnPostProcessInput.Broadcast(DeltaTime, bGamePaused);
}

void ACommonPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACommonPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
