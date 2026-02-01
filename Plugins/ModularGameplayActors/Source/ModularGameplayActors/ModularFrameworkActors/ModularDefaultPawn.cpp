// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularDefaultPawn.h"

#include "Components/GameFrameworkComponentManager.h"

AModularDefaultPawn::AModularDefaultPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AModularDefaultPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void AModularDefaultPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

