#pragma once
#include "BasicCoroutine/Core/Private.h"
#include "BasicCoroutine/Core/Promise.h"

template<typename T, typename... Args>
struct std::coroutine_traits<TCoroTask<T>, Args...>
{
	using promise_type = Coro::Private::TPromise<T>;
};