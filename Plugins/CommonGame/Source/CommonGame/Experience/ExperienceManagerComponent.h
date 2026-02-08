// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCoroutine/Coro.h"
#include "Components/GameStateComponent.h"
#include "ExperienceManagerComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ExperienceManagerLog, Log, All);

class UExperienceDefinition;

UENUM(BlueprintType)
enum class EExperienceLoadState : uint8
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UExperienceDefinition*);

UCLASS()
class COMMONGAME_API UExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:

	UExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Network Setting
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Experience's Section
	void SetCurrentExperienceAuth(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const { return LoadState == EExperienceLoadState::Loaded; }
	const UExperienceDefinition* GetCurrentExperienceChecked() const;
	
	/** Experience 로드 완료까지 대기합니다 (높은 우선순위) */
	static TCoroTask<const UExperienceDefinition*> WaitForExperienceLoaded_HighStaticCoroutine(UObject* WorldContextObject);

	/** Experience 로드 완료까지 대기합니다 (일반 우선순위) */
	static TCoroTask<const UExperienceDefinition*> WaitForExperienceLoadedStaticCoroutine(UObject* WorldContextObject);

	/** Experience 로드 완료까지 대기합니다 (낮은 우선순위) */
	static TCoroTask<const UExperienceDefinition*> WaitForExperienceLoaded_LowStaticCoroutine(UObject* WorldContextObject);
	
protected:
	UFUNCTION()
	void OnRep_CurrentExperienceId();
	
private:
	static TCoroTask<const UExperienceDefinition*> WaitForExperienceLoadedInternalCoroutine(UObject* WorldContextObject, int32 Priority);
	
	TCoroTask<void> LoadExperienceCoroutine();
	
	TCoroTask<void> LoadGameFeatureCoroutine(FString PluginURL) const;
	
	void DeactivateExperience();

private:
	// Experience 는 서버와 클라이언트 모두 동일해야 하므로 복제
	UPROPERTY(ReplicatedUsing = OnRep_CurrentExperienceId)
	FPrimaryAssetId CurrentExperienceId;
	
	// 메모리에 Loaded 된 Experience Asset 실물
	UPROPERTY()
	TObjectPtr<const UExperienceDefinition> CurrentExperience;
	
	// ExperienceLoadState 는 초기값이 Unloaded 
	EExperienceLoadState LoadState = EExperienceLoadState::Unloaded;
	
	TArray<FString> GameFeaturePluginURLs;
	
	// ExperienceLoad 완료 Delegate
	FOnExperienceLoaded OnExperienceLoaded_High;
	FOnExperienceLoaded OnExperienceLoaded_Normal;
	FOnExperienceLoaded OnExperienceLoaded_Low;
};
