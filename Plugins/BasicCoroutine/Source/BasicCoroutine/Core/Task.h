#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include "BasicCoroutine/Core/Context.h"

template<>
class TCoroTask<void>
{
public:
	TCoroTask() = default;
	explicit TCoroTask(FCoroContextPtr InContext) : Context(InContext) {}
	
	// 상태 읽기
	bool IsValid() const { return Context.IsValid(); }
	bool IsDone() const { return Context && Context->IsDone(); }
	
	// 상태 쓰기
	void Cancel()
	{
		if (Context)
		{
			Context->Cancel();
		}
	}
	
protected:
	FCoroContextPtr Context;
};

template<typename T>
class TCoroTask : public TCoroTask<void>
{
public:
	explicit TCoroTask(FCoroContextPtr InContext) : TCoroTask(InContext) {}
};