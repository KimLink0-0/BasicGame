// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAssetManager.h"

#include "CommonGame/PrimaryDataAsset/CommonPrimaryDataAsset.h"

UCommonAssetManager& UCommonAssetManager::Get()
{
	UAssetManager* AssetManager = GetIfInitialized();
	
	if (UCommonAssetManager* CommonAssetManager = Cast<UCommonAssetManager>(AssetManager))
	{
		return *CommonAssetManager;
	}
	
	checkf(false, TEXT("Failed to cast AssetManager to UCommonAssetManager. Please check the 'DefaultEngine.ini' settings."));
	
	// 컴파일용 return 코드
	return *NewObject<UCommonAssetManager>();
}

TSharedPtr<FStreamableHandle> UCommonAssetManager::ChangeBundleStateForPrimaryAssets(const TArray<FPrimaryAssetId>& AssetsToChange, const TArray<FName>& AddBundles, const TArray<FName>& RemoveBundles, bool bRemoveAllBundles, FAssetManagerLoadParams&& LoadParams, UE::FSourceLocation Location)
{
	// 상태가 변화할 예정인 AssetId 를 전부 LoadedAssets 에서 제거
	if (bRemoveAllBundles || RemoveBundles.Num() > 0 || AddBundles.Num() > 0)
	{
		for (const FPrimaryAssetId& AssetId : AssetsToChange)
		{
			LoadedAssets.Remove(AssetId);
		}
	}
	
	// 추가할 번들이 있다면, LoadedAssets 에 AssetId 추가하는 로직
	if (AddBundles.Num() > 0)
	{
		TArray<FPrimaryAssetId> AssetsToChangeCopy = AssetsToChange;
		
		// 기존 델리게이트 데이터 옮기기
		FStreamableDelegateWithHandle OriginalDelegate = MoveTemp(LoadParams.OnComplete);

		// 로딩이 완료되면 실행할 델리게이트 값 덮어 씌우기
		LoadParams.OnComplete = FStreamableDelegateWithHandle::CreateLambda(
			[this, AssetsToChangeCopy, OriginalDelegate](TSharedPtr<FStreamableHandle>)
		{
			
			for (const FPrimaryAssetId& AssetId : AssetsToChangeCopy)
			{
				// AssetId 로 활성화 되어있는 Bundles 를 역추적 하는 기능
				TArray<FName> CurrentActiveBundles;
				GetPrimaryAssetHandle(AssetId, true, &CurrentActiveBundles);
				
				// CurrentActiveBundles 의 숫자가 1개 이상이면, 현재 사용 중인 번들의 AssetId 이기에 LoadedAssets 목록에 추가 
				if (CurrentActiveBundles.Num() > 0)
				{
					LoadedAssets.Add(AssetId);	
				}
			}

			// 기존 델리게이트 실행
			if (OriginalDelegate.IsBound())
			{
				OriginalDelegate.Execute(nullptr);
			}
		});
	}

	return Super::ChangeBundleStateForPrimaryAssets(AssetsToChange, AddBundles, RemoveBundles, bRemoveAllBundles, MoveTemp(LoadParams), Location);
}

void UCommonAssetManager::LoadBundle(const UCommonPrimaryDataAsset* Asset)
{
	if (!Asset)
	{
		UE_LOG(LogTemp, Warning, TEXT("CommonAssetManager: LoadBundle 의 Asset 이 nullptr 입니다."))
		return;
	}
	
	const FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
	
	// 나중에 서버 전용도 아니고, 클라이언트 전용도 아닌 일반 게임 빌드? 가 있는지 확인 할 것
	TArray<FName> ServerOrClientBundles;
	// GameFeature 가 추가되면 로직 변경
	{
		bool bLoadServerData = false, bLoadClientData = false;
	
		if (FPlatformProperties::IsServerOnly())
		{
			bLoadServerData = true;
			bLoadClientData = false;
		}
	
		if (FPlatformProperties::IsClientOnly())
		{
			bLoadServerData = false;
			bLoadClientData = true;
		}
		
		
#if WITH_EDITORONLY_DATA
		bLoadServerData = (StaticClientOnlyCheck() == 0);
		bLoadClientData = (StaticDedicatedServerCheck() == 0);
#endif
		
	
		if (bLoadServerData)
		{
			ServerOrClientBundles.Add(TEXT("Server"));
		}
	
		if (bLoadClientData)
		{
			ServerOrClientBundles.Add(TEXT("Client"));
		}
	}
	
	FAssetManagerLoadParams LoadParams;
	TSharedPtr<FStreamableHandle> Handle = ChangeBundleStateForPrimaryAssets(
		{ AssetId },
		ServerOrClientBundles,
		{},
		false,
		MoveTemp(LoadParams)
		);
	
	if (Handle.IsValid())
	{
		Handle->WaitUntilComplete();
	}
}
