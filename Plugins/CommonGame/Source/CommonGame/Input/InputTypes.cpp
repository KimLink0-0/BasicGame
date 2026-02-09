// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTypes.h"
#include "InputAction.h"

FString FInputTypes::ToString() const
{
	return FString::Printf(TEXT("Tag: [%s] -> Action: [%s]"), *InputTag.ToString(), InputAction ? *InputAction->GetName() : TEXT("None"));
}
