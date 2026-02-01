// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularPlayerState.h"

#include "Components/GameFrameworkComponentManager.h"

AModularPlayerState::AModularPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AModularPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerState::BeginPlay()
{
	Super::BeginPlay();
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
}

void AModularPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
