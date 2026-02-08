// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "CommonWorldSettings.generated.h"

class UExperienceDefinition;

UCLASS()
class COMMONGAME_API ACommonWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	ACommonWorldSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	FPrimaryAssetId GetDefaultExperienceId() const { return DefaultExperienceId; }
	
public:
	// ExperienceDefinition 타입만 등록하고 싶으니까, ExperienceDefinition 전방 선언! 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Experience", meta = (AllowedTypes = "ExperienceDefinition"))
	FPrimaryAssetId DefaultExperienceId;
};
