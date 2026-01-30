#pragma once

#include "CoreMinimal.h"
#include <coroutine>

namespace Coro::Private
{
	// Context 전방 선언
	struct FCoroContext;
	
	template<typename T>
	struct TCoroContext;
}