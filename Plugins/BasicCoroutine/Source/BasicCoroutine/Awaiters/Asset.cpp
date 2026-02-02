// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset.h"

#include "GameFeaturePluginOperationResult.h"
#include "GameFeaturesSubsystem.h"

namespace Coro::Private
{
	FLoadPrimaryAssetsAwaiter::FLoadPrimaryAssetsAwaiter(UObject* InOwner, TArray<FPrimaryAssetId> PrimaryAssetIds, TArray<FName> InBundles) : 
		Super(InOwner),
		AssetIds(MoveTemp(PrimaryAssetIds)),
		Bundles(InBundles)
	{
	}

	bool FLoadPrimaryAssetsAwaiter::Ready() const
	{
		if (AssetIds.IsEmpty())
		{
			return true;
		}
		
		UAssetManager& AssetManager = UAssetManager::Get();
		
		for (const FPrimaryAssetId& AssetId : AssetIds)
		{
			// 모든 에셋이 로딩된 게 아니라면 로딩 진행 (하나라도 덜 된 게 있다면 로딩) 
			if (!AssetManager.GetPrimaryAssetObject(AssetId))
			{
				return false;
			}
		}
		
		return true;
	}

	void FLoadPrimaryAssetsAwaiter::Suspend()
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		
		StreamHandle = AssetManager.LoadPrimaryAssets(
			AssetIds,
			Bundles,
			FStreamableDelegate::CreateLambda([CapturedOwner = Owner, CapturedContext = Context]()
			{
				SafeResume(CapturedOwner, CapturedContext);
			})
		);
	}

	FLoadGameFeatureAwaiter::FLoadGameFeatureAwaiter(UObject* InOwner, FString InPluginURL) :
		Super(InOwner),
		PluginURL(InPluginURL)
	{
	}

	bool FLoadGameFeatureAwaiter::Ready() const
	{
		return false;
	}

	void FLoadGameFeatureAwaiter::Suspend()
	{
		UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(
		PluginURL,
		FGameFeaturePluginLoadComplete::CreateLambda([CapturedOwner = Owner, CapturedContext = Context, SuccessPtr = &bSuccess](const UE::GameFeatures::FResult& Result)
			{
				if (CapturedOwner.IsValid())
				{
					*SuccessPtr = Result.HasValue();
				}
				SafeResume(CapturedOwner, CapturedContext);
			})
			);
	}

	bool FLoadGameFeatureAwaiter::GetResult() const
	{
		return bSuccess;
	}
}

