// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceDefinition.h"

UExperienceDefinition::UExperienceDefinition()
{
}

FPrimaryAssetId UExperienceDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType("ExperienceDefinition"), GetFName());
}
