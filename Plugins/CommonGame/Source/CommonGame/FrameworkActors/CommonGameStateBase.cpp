// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameStateBase.h"

#include "CommonGame/Experience/ExperienceManagerComponent.h"

ACommonGameStateBase::ACommonGameStateBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
