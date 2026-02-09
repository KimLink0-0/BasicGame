// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorExtension.h"
#include "CommonCoroutine/Coro.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorExtensionWorldSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ActorExtensionWorldSubsystemLog, Log, All);

class UExperienceDefinition;
struct FComponentRequestHandle;

// Register 상태 Actor 데이터
struct FRegisterActorData
{
	TWeakObjectPtr<AActor> Actor;
	TArray<FActorExtension> Extensions;
};

// Complete 상태 Actor 데이터
struct FCompleteActorData
{
	TArray<FActorExtension> Extensions;	
};

// Extension + 네트워크 Role 설정 
struct FExtensionEntry
{
	FActorExtension Extension;
	bool bAddToLocallyControlled = false;
	bool bAddToSimulatedProxy = false;
};

// Extension 정의 Caching
struct FClassExtensionMapping
{
	TArray<FExtensionEntry> Entries;
};


UCLASS()
class COMMONGAME_API UActorExtensionWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// TickableWorldSubsystem's Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual bool IsTickable() const override;
	
	// Extension 등록과 해제
	void RegisterExtensionForClass(UClass* TargetClass, const FActorExtension& Extension, bool bAddToLocallyControlled, bool bAddToSimulatedProxy);
	void UnregisterExtensionsForClass(UClass* TargetClass);
	
	// Extension 활성화 완료 확인
	bool IsActorComplete(const AActor* Actor) const;
	
private:
	
	TCoroTask<void> WaitForExperienceLoadedCoroutine();
	
	void RegisterWithGameFrameworkComponentManager();
	
	void HandleActorExtensionEvent(AActor* Actor, FName EventName);
	
	// Mapping 확인 후 Extension 복사본 생성
	bool TryCreateExtensions(AActor* Actor, TArray<FActorExtension>& OutExtensions);
	
	void RemoveActor(AActor* Actor);
	
	
private:
	TArray<TWeakObjectPtr<AActor>> UncheckedActors;
	
	TArray<FRegisterActorData> RegisterActors;
	
	TMap<TWeakObjectPtr<AActor>, FCompleteActorData> CompleteActors;
	
	TMap<UClass*, FClassExtensionMapping> ClassExtensionCache;
	
	bool bExperienceLoaded = false;
	
	TArray<TSharedPtr<FComponentRequestHandle>> ExtensionHandles;
	
};
