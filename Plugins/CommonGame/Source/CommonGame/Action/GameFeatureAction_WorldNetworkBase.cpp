// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_WorldNetworkBase.h"

#include "GameFeaturesSubsystemSettings.h"
#include "CommonGame/Utility/SoftPathCollector.h"
#include "Engine/AssetManager.h"

UGameFeatureAction_WorldNetworkBase::UGameFeatureAction_WorldNetworkBase() :
	bServerAction(false),
	bClientAction(false)
{
}

void UGameFeatureAction_WorldNetworkBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FGameFeatureStateChangeContext ChangeContext(Context);
	
	// OnStartGameInstance: GameInstance 가 실행 될 때 호출되는 WorldDelegate 에 바인딩
	// 1. 새로 참여하는 Player 를 위한 델리게이트
	FDelegateHandle StartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &ThisClass::HandleGameInstanceStart, ChangeContext);
	GameInstanceStartHandles.Add(ChangeContext, StartHandle);
	
	// OnGameInstanceWorldChanged : GameInstance 의 World 가 변경 될 때 호출되는 WorldDelegate 에 바인딩
	// 2. 레벨 이동을 위한 델리게이트
	FDelegateHandle WorldChangedHandle = FWorldDelegates::OnGameInstanceWorldChanged.AddUObject(this, &ThisClass::HandleGameInstanceWorldChanged, ChangeContext);
	GameInstanceWorldChangeHandles.Add(ChangeContext, WorldChangedHandle);
	
	// 현재 존재하는 모든 World 에서 AddToWorld 실행
	// 3. PIE 모드에서 켜져있는 모든 창들 (Server, Client1, Client2 ...) 들의 World 들에서 AddToWorld 를 실행
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			UWorld* World = WorldContext.World();
			if (World && World->IsGameWorld() && ShouldApplyToNetMode(World->GetNetMode()))
			{
				AddToWorld(WorldContext, ChangeContext);
			}
		}
	}
}

void UGameFeatureAction_WorldNetworkBase::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FGameFeatureStateChangeContext ChangeContext(Context);
	
	// OnStartGameInstance 델리게이트 해제 및 배열에서 삭제
	if (FDelegateHandle* HandlePtr = GameInstanceStartHandles.Find(ChangeContext))
	{
		FWorldDelegates::OnStartGameInstance.Remove(*HandlePtr);
		GameInstanceStartHandles.Remove(ChangeContext);
	}
	
	// OnGameInstanceWorldChanged 델리게이트 해제 및 배열에서 삭제
	if (FDelegateHandle* HandlePtr = GameInstanceWorldChangeHandles.Find(ChangeContext))
	{
		FWorldDelegates::OnStartGameInstance.Remove(*HandlePtr);
		GameInstanceWorldChangeHandles.Remove(ChangeContext);
	}
}

void UGameFeatureAction_WorldNetworkBase::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (!UAssetManager::IsInitialized())
	{
		return;
	}
	
	TArray<FSoftObjectPath> ServerPaths;
	TArray<FSoftObjectPath> ClientPaths;
	FSoftPathCollector::CollectSoftObjectPaths(GetClass(), this, ClientPaths, ServerPaths);
	
	for (const FSoftObjectPath& Path : ServerPaths)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, Path.GetAssetPath());
	}
	
	for (const FSoftObjectPath& Path : ClientPaths)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Path.GetAssetPath());
	}
}

bool UGameFeatureAction_WorldNetworkBase::ShouldApplyToNetMode(ENetMode NetMode) const
{	
	switch (NetMode)
	{
	case NM_Client:
		return bClientAction;
	case NM_DedicatedServer:
		return bServerAction;
	case NM_ListenServer:
	case NM_Standalone:
		return bClientAction || bServerAction;
	default:
		return false;
	}
}

void UGameFeatureAction_WorldNetworkBase::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
{
	if (const FWorldContext* WorldContext = GameInstance->GetWorldContext())
	{
		UWorld* World = WorldContext->World();
		if (World && World->IsGameWorld() && ChangeContext.ShouldApplyToWorldContext(*WorldContext) && ShouldApplyToNetMode(World->GetNetMode()))
		{
			AddToWorld(*WorldContext, ChangeContext);
		}
	}
}

void UGameFeatureAction_WorldNetworkBase::HandleGameInstanceWorldChanged(UGameInstance* GameInstance, UWorld* OldWorld, UWorld* NewWorld, FGameFeatureStateChangeContext ChangeContext)
{
	if (!NewWorld || !NewWorld->IsGameWorld())
	{
		return;
	}
	
	const FWorldContext* WorldContext = GameInstance->GetWorldContext();
	if (!WorldContext)
	{
		return;
	}
	
	if (ChangeContext.ShouldApplyToWorldContext(*WorldContext) && ShouldApplyToNetMode(NewWorld->GetNetMode()))
	{
		AddToWorld(*WorldContext, ChangeContext);
	}
}
