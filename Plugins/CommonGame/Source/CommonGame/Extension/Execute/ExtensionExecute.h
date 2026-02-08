#pragma once

#include "CoreMinimal.h"
#include "ExtensionExecute.generated.h"

class AActor;

USTRUCT(BlueprintType, meta=(Hidden))
struct COMMONGAME_API FExtensionExecute
{
	GENERATED_BODY()
public:
	virtual ~FExtensionExecute() = default;
	
	virtual void OnActivate(AActor* Owner) const {}
	
	virtual void OnDeactivate(AActor* Owner) const {}
};