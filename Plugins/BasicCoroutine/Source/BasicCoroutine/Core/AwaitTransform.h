#pragma once

#include "Core/Promise.h"

template<typename T>
decltype(auto) Coro::Private::FPromise::await_transform(T&& Awaitable)
{
    return std::forward<T>(Awaitable);
}