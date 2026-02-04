// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonPrimaryDataAsset.generated.h"

// TSoftClassPtr, TSoftObjectPtr 를 통한 SoftReference 사용 : SoftPtr 은 내부적으로 FSoftObjectPath 를 사용
// meta = (AssetBundles = "Client")
// meta = (AssetBundles = "Server")
// meta = (AssetBundles = "All")

UCLASS(Abstract)
class COMMONGAME_API UCommonPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
#if WITH_EDITORONLY_DATA
	
	virtual void UpdateAssetBundleData() override;
	
	const FAssetBundleData& GetAssetBundleData() const { return AssetBundleData; }

#endif
};
