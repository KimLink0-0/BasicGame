// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldNetworkBase.h"
#include "CommonGame/Extension/ActorExtension.h"
#include "GameFeatureAction_AddActorExtension.generated.h"


UCLASS(meta = (DisplayName = "Add Actor Extension"))
class COMMONGAME_API UGameFeatureAction_AddActorExtension : public UGameFeatureAction_WorldNetworkBase
{
	GENERATED_BODY()
	
public:
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	
private:
	void RegisterExtension(UWorld* World);
	
	void UnregisterExtension(UWorld* World);
	
public:
	UPROPERTY(EditAnywhere, Category = "Extension", meta = (AllowAbstract = "false"))
	TSubclassOf<AActor> TargetClass;
	
	UPROPERTY(EditAnywhere, Category = "Extension")
	FActorExtension Extension;
	
	UPROPERTY(EditAnywhere, Category = "Network", meta = (EditCondition = "bClientAction"))
	uint8 bAddToLocallyControlled : 1 = false;
	
	UPROPERTY(EditAnywhere, Category = "Network", meta = (EditCondition = "bClientAction"))
	uint8 bAddToSimulatedProxy : 1 = false;
};
