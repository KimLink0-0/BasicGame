// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonCoroutine/Coro.h"
#include "ModularGameplayActors/ModularFrameworkActors/ModularGameModeBase.h"
#include "CommonGameModeBase.generated.h"

class UExperienceDefinition;
DECLARE_LOG_CATEGORY_EXTERN(CommonGameModeLog, Log, All);

UCLASS()
class COMMONGAME_API ACommonGameModeBase : public AModularGameModeBase
{
	GENERATED_BODY()
public:
	ACommonGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Experience 설정 Point
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	// Experience 로드 전에 Pawn 설정 되는 것을 방지
	// GameModeBase's Interface
	// 1. "RestartPlayer (플레이어 생성) 지시"
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	// 2. 1. 안에서 실행 되며 "Default Pawn 데이터를 설정해주는 함수"
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	// 3. 1. 안에서 실행 되며 2. 데이터를 기반으로 "Pawn 을 소환해주는 함수"
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	

protected:
	bool IsExperienceLoaded() const;
	
	void OnExperienceLoaded(const UExperienceDefinition* Experience);
	
private:
	TCoroTask<void> StartExperienceLoadCoroutine();

};
