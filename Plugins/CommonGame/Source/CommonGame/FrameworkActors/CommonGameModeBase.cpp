// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameModeBase.h"

#include "CommonCharacter.h"
#include "CommonGameStateBase.h"
#include "CommonHUD.h"
#include "CommonPlayerController.h"
#include "CommonPlayerState.h"
#include "BasicCoroutine/Awaiters/Time.h"
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

TCoroTask<void> ACommonGameModeBase::StartExperienceLoadCoroutine()
{
	// 1. GameState 준비: ExperienceManagerComponent 는 GameStateComponent 를 상속 받아서
	co_await Coro::Latent::Until(this, [this]()
	{
		return GetGameState<ACommonGameStateBase>() != nullptr;
	});
	
	// 2. WorldSettings 준비: 맵에 설정 된 ExperienceDefinition 가져오기 
	ACommonWorldSettings* CommonWorldSettings = Cast<ACommonWorldSettings>(GetWorld()->GetWorldSettings());
	FPrimaryAssetId ExperienceId;
	{
		if (!CommonWorldSettings)
		{
			UE_LOG(CommonGameModeLog, Error, TEXT("CommonWorldSettings 을 확인 해주세요"));
			co_return;
		}
		ExperienceId = CommonWorldSettings->GetDefaultLevelExperienceId();
	}
	
	// 3. ExperienceManager 를 통해 CurrentExperience 설정
	ACommonGameStateBase* CommonGameState = GetGameState<ACommonGameStateBase>();
	UExperienceManagerComponent* ExperienceManagerComponent = CommonGameState->GetExperienceManagerComponent();
	if (!ExperienceManagerComponent)
	{
		UE_LOG(CommonGameModeLog, Error, TEXT("CommonGameStateBase 에서 ExperienceManagerComponent 설정을 확인 해주세요"));
		co_return;
	}
	
	// 4. Experience Loading 시작 : On_Rep 함수를 트리거로 시작
	ExperienceManagerComponent->SetCurrentExperienceAuth(ExperienceId);
	
	// 5. Experience 로딩 대기 : Load 완료 시 델리게이트 호출 
	if (const UExperienceDefinition* Experience = co_await UExperienceManagerComponent::WaitForExperienceLoadedStaticCoroutine(this))
	{
		// OnExperienceLoaded(Experience);
	}
}
