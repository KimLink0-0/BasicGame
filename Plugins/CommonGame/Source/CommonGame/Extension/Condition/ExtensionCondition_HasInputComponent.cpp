// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtensionCondition_HasInputComponent.h"

bool FExtensionCondition_HasInputComponent::IsSatisfied(AActor* Owner) const
{
	APawn* Pawn = Cast<APawn>(Owner);
	if (!Pawn || !Pawn->InputComponent)
	{
		return false;
	}
	
	if (RequiredClass)
	{
		return Pawn->InputComponent->IsA(RequiredClass);
	}
	
	return true;
}
