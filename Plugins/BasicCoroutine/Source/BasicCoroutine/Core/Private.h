#pragma once

#include "CoreMinimal.h"
#include <coroutine>

template<typename T = void>
class TCoroTask;

namespace Coro::Private
{
	// Context 전방 선언
	struct FCoroContext;
	
	template<typename T>
	struct TCoroContext;
	
	// Promise 전방 선언
	class FPromise;
	
	template<typename T>
	class TPromise;
}

using FCoroContextPtr = TSharedPtr<Coro::Private::FCoroContext, ESPMode::ThreadSafe>;

template<typename T>
using TCoroContextPtr = TSharedPtr<Coro::Private::TCoroContext<T>, ESPMode::ThreadSafe>;

template<typename T>
using TCoroContextWeakPtr = TWeakPtr<Coro::Private::TCoroContext<T>, ESPMode::ThreadSafe>;