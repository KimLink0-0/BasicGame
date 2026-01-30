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

using FCoroContextPtr = TSharedPtr<Coro::Private::FCoroContext, ESPMode::ThreadSafe>;

template<typename T>
using TCoroContextPtr = TSharedPtr<Coro::Private::TCoroContext<T>, ESPMode::ThreadSafe>;

template<typename T>
using TCoroContextWeakPtr = TWeakPtr<Coro::Private::TCoroContext<T>, ESPMode::ThreadSafe>;