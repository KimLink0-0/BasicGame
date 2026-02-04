// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPrimaryDataAsset.h"

#include "CommonGame/Utility/SoftPathCollector.h"
#include "Engine/AssetManager.h"

#if WITH_EDITORONLY_DATA

void UCommonPrimaryDataAsset::UpdateAssetBundleData()
{
	// 에셋 매니저가 없으면 리턴
	if (!UAssetManager::IsInitialized())
	{
		return;
	}
	
	
	AssetBundleData.Reset();
	
	TArray<FSoftObjectPath> ClientPaths;
	TArray<FSoftObjectPath> ServerPaths;
	FSoftPathCollector::CollectSoftObjectPaths(GetClass(), this, ClientPaths, ServerPaths);
	
	for (const FSoftObjectPath& Path : ClientPaths)
	{
		AssetBundleData.AddBundleAsset(FName(TEXT("Client")), Path.GetAssetPath());
	}
	
	for (const FSoftObjectPath& Path : ServerPaths)
	{
		AssetBundleData.AddBundleAsset(FName(TEXT("Server")), Path.GetAssetPath());
	}
}

#endif
