// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceManagerComponent.h"

#include "BasicCoroutine/Awaiters/Time.h"
#include "ExperienceDefinition.h"
#include "BasicCoroutine/Awaiters/Asset.h"
#include "BasicCoroutine/Awaiters/Delegate.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

DEFINE_LOG_CATEGORY(ExperienceManagerLog);

UExperienceManagerComponent::UExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UExperienceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentExperienceId, Params);
}

void UExperienceManagerComponent::SetCurrentExperienceAuth(FPrimaryAssetId ExperienceId)
{
	check(GetOwner()->HasAuthority());
	check(LoadState == EExperienceLoadState::Unloaded)
	
	CurrentExperienceId = ExperienceId;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentExperienceId, this);
}

const UExperienceDefinition* UExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check (LoadState == EExperienceLoadState::Loaded);
	check (CurrentExperience != nullptr);
	return CurrentExperience;
}

TCoroTask<const UExperienceDefinition*> UExperienceManagerComponent::WaitForExperienceLoadedStaticCoroutine(
	UObject* WorldContextObject)
{
	co_return co_await WaitForExperienceLoadedInternalCoroutine(WorldContextObject);
}

void UExperienceManagerComponent::OnRep_CurrentExperienceId()
{
	LoadExperienceCoroutine();
}

TCoroTask<const UExperienceDefinition*> UExperienceManagerComponent::WaitForExperienceLoadedInternalCoroutine(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		co_return nullptr;	
	}
	
	UExperienceManagerComponent* ExperienceManager = nullptr;
	while (!ExperienceManager)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (!World)
		{
			co_return nullptr;
		}
		
		if (AGameStateBase* GameState = World->GetGameState())
		{
			ExperienceManager = GameState->FindComponentByClass<UExperienceManagerComponent>();
		}
		
		if (!ExperienceManager)
		{
			co_await Coro::Latent::NextTick(WorldContextObject);
		}
	}
	
	if (ExperienceManager->IsExperienceLoaded())
	{
		co_return ExperienceManager->GetCurrentExperienceChecked();
	}
	
	co_return co_await Coro::Async::WaitForDelegate(ExperienceManager, ExperienceManager->OnExperienceLoaded);
	
}

TCoroTask<void> UExperienceManagerComponent::LoadExperienceCoroutine()
{
	// 1. 로딩이 안됐거나 비활성화 상태이면 시작
	check(LoadState == EExperienceLoadState::Unloaded || LoadState == EExperienceLoadState::Deactivating)
	check(CurrentExperienceId.IsValid());
	
	// 대부분의 경우 GameState 생성된 다음 Tick 에 Pawn 생성 준비를 완료 : Experience 는 Pawn 에 대한 데이터를 설정해야 하니까
	co_await Coro::Latent::NextTick(this);
	
	// 로딩 시작할게
	LoadState = EExperienceLoadState::Loading;
	
	CurrentExperience = co_await Coro::Async::LoadPrimaryAsset<UExperienceDefinition>(this, CurrentExperienceId, {});
	
	if (!CurrentExperience)
	{
		UE_LOG(ExperienceManagerLog, Error, TEXT("%s Experience 에셋 로드 실패"), *CurrentExperienceId.ToString())
		co_return;
	}
	
	// 게임 피쳐 로딩 로직 추가 예정
	// LoadState = EExperienceLoadState::LoadingGameFeatures;
	// GameFeaturePluginURLs.Reset();
	
	// Experience Load 완료 : 상태 Loaded 로 변경 및 Delegate Broadcast 진행
	LoadState = EExperienceLoadState::Loaded;
	
	UE_LOG(ExperienceManagerLog, Log, TEXT("%s Experience 에셋 로드 완료"), *CurrentExperienceId.ToString())
	
	OnExperienceLoaded.Broadcast(CurrentExperience);
}
