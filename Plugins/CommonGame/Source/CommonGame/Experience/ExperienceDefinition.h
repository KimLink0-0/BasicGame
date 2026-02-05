// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExperienceDefinition.generated.h"


UCLASS(BlueprintType, Const)
class COMMONGAME_API UExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UExperienceDefinition();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<APawn> DefaultPawnClass;
	
	// GameFeatureData 를 상속받는 타입만을 리스트에 추가할 수 있음
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Features", meta = (AllowedTypes = "GameFeatureData"))
	TArray<FPrimaryAssetId> GameFeaturesToEnable;
};
