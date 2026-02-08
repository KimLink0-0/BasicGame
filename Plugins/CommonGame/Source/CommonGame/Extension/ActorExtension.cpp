// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorExtension.h"

#include "StructUtils/InstancedStruct.h"


bool FActorExtension::CanActivate(AActor* Owner) const
{
	for (const auto& ConditionStruct : Conditions)
	{
		if (const FExtensionCondition* Condition = ConditionStruct.GetPtr())
		{
			if (!Condition->IsSatisfied(Owner))
			{
				return false;
			}
		}
	}
	
	return true;
}

void FActorExtension::OnActivate(AActor* Owner)
{
	if (bActivated)
	{
		return;
	}
	
	bActivated = true;
	
	for (const auto& ExecuteStruct : Executes)
	{
		if (const FExtensionExecute* Execute = ExecuteStruct.GetPtr())
		{
			Execute->OnActivate(Owner);
		}
	}
}

void FActorExtension::OnDeactivate(AActor* Owner)
{
	if (!bActivated)
	{
		return;
	}
	
	bActivated = false;
	
	for (const auto& ExecuteStruct : Executes)
	{
		if (const FExtensionExecute* Execute = ExecuteStruct.GetPtr())
		{
			Execute->OnDeactivate(Owner);
		}
	}
}
