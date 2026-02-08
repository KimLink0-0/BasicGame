// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameModeBase.h"

#include "CommonCharacter.h"
#include "CommonGameStateBase.h"
#include "CommonHUD.h"
#include "CommonPlayerController.h"
#include "CommonPlayerState.h"
#include "CommonCoroutine/Awaiters/Time.h"
#include "CommonGame/Experience/ExperienceDefinition.h"
#include "CommonGame/Experience/ExperienceManagerComponent.h"
#include "CommonGame/WorldSettings/CommonWorldSettings.h"

DEFINE_LOG_CATEGORY(CommonGameModeLog);

ACommonGameModeBase::ACommonGameModeBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	DefaultPawnClass = ACommonCharacter::StaticClass();
	HUDClass = ACommonHUD::StaticClass();
	PlayerControllerClass = ACommonPlayerController::StaticClass();
	GameStateClass = ACommonGameStateBase::StaticClass();
	PlayerStateClass = ACommonPlayerState::StaticClass();
}

void ACommonGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	StartExperienceLoadCoroutine();
}

void ACommonGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (!IsExperienceLoaded())
	{
		UE_LOG(CommonGameModeLog, Log, TEXT("Experience 로드 대기 중, 플레이어 처리 지연: %s"), *GetNameSafe(NewPlayer));
		return;
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

UClass* ACommonGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (!IsExperienceLoaded())
	{
		return nullptr;
	}
	
	if (ACommonGameStateBase* CommonGameState = GetGameState<ACommonGameStateBase>())
	{
		if (UExperienceManagerComponent* ExperienceManager = CommonGameState->GetExperienceManagerComponent())
		{
			if (const UExperienceDefinition* Experience = ExperienceManager->GetCurrentExperienceChecked())
			{
				if (Experience->DefaultPawnClass)
				{
					return Experience->DefaultPawnClass;
				}
			}
		}
	}
	
	UE_LOG(CommonGameModeLog, Warning, TEXT("Experience 에서 DefaultPawnClass 불러오기 실패"));
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ACommonGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (!IsExperienceLoaded())
	{
		UE_LOG(CommonGameModeLog, Log, TEXT("Experience 로드 대기 중, DefaultPawn Spawn 지연"));
		return nullptr;
	}
	
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

bool ACommonGameModeBase::IsExperienceLoaded() const
{
	if (ACommonGameStateBase* CommonGameState = GetGameState<ACommonGameStateBase>())
	{
		if (UExperienceManagerComponent* ExperienceManager = CommonGameState->GetExperienceManagerComponent())
		{
			return ExperienceManager->IsExperienceLoaded();
		}
	}

	return false;
}

void ACommonGameModeBase::OnExperienceLoaded(const UExperienceDefinition* Experience)
{
	UE_LOG(CommonGameModeLog, Log, TEXT("Experience 로드 완료: %s"), *Experience->GetName());
	
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++ Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

TCoroTask<void> ACommonGameModeBase::StartExperienceLoadCoroutine()
{
	// GameState 생성까지 대기합니다
	co_await Coro::Latent::Until(this, [this]() { return GetGameState<ACommonGameStateBase>() != nullptr; });
	
	ACommonGameStateBase* CommonGameState = GetGameState<ACommonGameStateBase>();
	UExperienceManagerComponent* ExperienceManager = CommonGameState->GetExperienceManagerComponent();
	if (!ExperienceManager)
	{
		UE_LOG(CommonGameModeLog, Error, TEXT("ExperienceManagerComponent를 찾을 수 없습니다"));
		co_return;
	}
	
	// WorldSettings에서 Experience 가져오기
	ACommonWorldSettings* CommonWorldSettings = Cast<ACommonWorldSettings>(GetWorld()->GetWorldSettings());
	if (!CommonWorldSettings)
	{
		UE_LOG(CommonGameModeLog, Error, TEXT("CommonWorldSettings를 찾을 수 없습니다"));
		co_return;
	}
	
	FPrimaryAssetId ExperienceId = CommonWorldSettings->GetDefaultExperienceId();
	
	if (!ExperienceId.IsValid())
	{
		UE_LOG(CommonGameModeLog, Error, TEXT("WorldSettings에 DefaultExperienceId가 설정되지 않았습니다"));
		co_return;
	}

	// Experience 설정 시작
	UE_LOG(CommonGameModeLog, Log, TEXT("Experience 로드 시작: %s"), *ExperienceId.ToString());
	ExperienceManager->SetCurrentExperienceAuth(ExperienceId);

	// Experience 로드 완료 대기 (델리게이트 기반)
	if (const UExperienceDefinition* Experience = co_await UExperienceManagerComponent::WaitForExperienceLoaded_HighStaticCoroutine(this))
	{
		OnExperienceLoaded(Experience);
	}
}