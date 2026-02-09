// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddActorExtension.h"

#include "CommonGame/Extension/ActorExtensionWorldSubsystem.h"

void UGameFeatureAction_AddActorExtension::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (UWorld* World = WorldContext.World())
		{
			UnregisterExtension(World);
		}
	}
}

void UGameFeatureAction_AddActorExtension::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	if (!World)
	{
		return;
	}
	
	RegisterExtension(World);
}

void UGameFeatureAction_AddActorExtension::RegisterExtension(UWorld* World)
{
	if (!TargetClass)
	{
		return;
	}
	
	UActorExtensionWorldSubsystem* Subsystem = World->GetSubsystem<UActorExtensionWorldSubsystem>();
	if (!Subsystem)
	{
		TWeakObjectPtr WeakWorld = World;
		TWeakObjectPtr WeakThis = this;
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([WeakWorld, WeakThis]()
		{
			if (ThisClass* This = WeakThis.Get())
			{
				if (UWorld* World = WeakWorld.Get())
				{
					This->RegisterExtension(World);
				}
			}
		}));
		return;
	}
	
	Subsystem->RegisterExtensionForClass(TargetClass.Get(), Extension, bAddToLocallyControlled, bAddToSimulatedProxy);
}

void UGameFeatureAction_AddActorExtension::UnregisterExtension(UWorld* World)
{
	if (!TargetClass)
	{
		return;
	}
	
	UActorExtensionWorldSubsystem* Subsystem = World->GetSubsystem<UActorExtensionWorldSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	
	Subsystem->UnregisterExtensionsForClass(TargetClass.Get());
}
