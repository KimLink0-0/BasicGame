// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularPlayerController.h"

#include "Components/GameFrameworkComponentManager.h"

AModularPlayerController::AModularPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AModularPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void AModularPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
