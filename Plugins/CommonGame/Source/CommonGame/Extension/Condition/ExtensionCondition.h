#pragma once

#include "CoreMinimal.h"
#include "ExtensionCondition.generated.h"

class AActor;

USTRUCT(BlueprintType, meta=(Hidden))
struct COMMONGAME_API FExtensionCondition
{
	GENERATED_BODY()
public:
	virtual ~FExtensionCondition() = default;
	
	virtual bool IsSatisfied(AActor* Owner) const { return true; }
};