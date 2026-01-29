#pragma once

#include <coroutine>
#include "Task.h"
#include "Promise.h"

// [Traits 의 역할] 컴파일러에게 Task 와 Promise 를 연결하는 역할
template<typename T, typename... Args>
struct std::coroutine_traits<TCoroTask<T>, Args...>
{
	// TCoroTask<T> 를 반환하는 함수는 TCoroPromise<T> 를 사용하라고 알림
	using promise_type = Coro::Private::TCoroPromise<T>;
};