// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldNetworkBase.generated.h"

UCLASS(Abstract)
class COMMONGAME_API UGameFeatureAction_WorldNetworkBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	UGameFeatureAction_WorldNetworkBase();
	
	// GameFeatureAction's Interface 
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	
protected:
	// Member's Methods
	
	// UCLASS 에서 PURE_VIRTUAL 을 만드는 방법
	// 파생 클래스에서는 AddToWorld 를 통해서 해당 World 에서 Activating 을 시도하게 된다.
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldNetworkBase::AddToWorld, );
	
	bool ShouldApplyToNetMode(ENetMode NetMode) const;
	
private:
	// GameInstance 시작 시 호출되는 델리게이트
	void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);
	
	// GameInstance 의 World 변경 시 호출되는 델리게이트
	void HandleGameInstanceWorldChanged(UGameInstance* GameInstance, UWorld* OldWorld, UWorld* NewWorld, FGameFeatureStateChangeContext ChangeContext);
	
	
public:
	// Member's Variables
	UPROPERTY(EditAnywhere, Category = "Network")
	uint8 bServerAction : 1;
	
	UPROPERTY(EditAnywhere, Category = "Network")
	uint8 bClientAction : 1;
	
private:
	// GameInstance 가 실행될 때 호출되는 델리게이트 핸들 배열
	TMap<FGameFeatureStateChangeContext, FDelegateHandle> GameInstanceStartHandles;
	
	// GameInstance 의 World 가 교체될 때 호출되는 델리게이트 핸들 배열
	TMap<FGameFeatureStateChangeContext, FDelegateHandle> GameInstanceWorldChangeHandles;
	
};
