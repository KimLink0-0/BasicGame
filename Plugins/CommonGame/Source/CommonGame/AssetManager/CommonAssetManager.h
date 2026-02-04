// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGame/PrimaryDataAsset/CommonPrimaryDataAsset.h"
#include "Engine/AssetManager.h"
#include "CommonAssetManager.generated.h"


UCLASS()
class COMMONGAME_API UCommonAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UCommonAssetManager& Get();
	
	// Getter & Setter
	TSet<FPrimaryAssetId> GetLoadedAssets() { return LoadedAssets; }
	
	// Member Methods
	void LoadBundle(const UCommonPrimaryDataAsset* Asset);
	
	// AssetManager's Interface
	virtual TSharedPtr<FStreamableHandle> ChangeBundleStateForPrimaryAssets(const TArray<FPrimaryAssetId>& AssetsToChange, const TArray<FName>& AddBundles, const TArray<FName>& RemoveBundles, bool bRemoveAllBundles, FAssetManagerLoadParams&& LoadParams, UE::FSourceLocation Location = UE::FSourceLocation::Current()) override;

	
private:
	// Member Variables
	TSet<FPrimaryAssetId> LoadedAssets;
};
