#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include "BasicCoroutine/Core/Context.h"
#include "BasicCoroutine/Core/Task.h"
#include <coroutine>

namespace Coro::Private
{
	class FPromise
	{
	public:
		FPromise() = default;
		virtual ~FPromise()
		{
			if (Context)
			{
				Context->SetPromise(nullptr);
			}
		}
		
		void Resume() const { CoroutineHandle.resume(); }
		
		FCoroContextPtr GetContextForAwaiter() const { return Context;}
		
		// [필수] 시작 시 멈출 지 유무
		// suspend_never : 즉시 시작
		std::suspend_never initial_suspend() const { return {}; }
		
		// [필수] 종료 시 멈출 지 유무
		// suspend_never : 즉시 정지
		std::suspend_never final_suspend() const { return {}; }
		
		// [필수] 예외 처리 
		void unhandled_exception() { check(false); }
		
		void SetCoroutineHandle(std::coroutine_handle<> InHandle) { CoroutineHandle = InHandle; }
		
	protected:
		FCoroContextPtr Context;
		
		std::coroutine_handle<> CoroutineHandle;
	};
		
	template<typename T>
	class TPromise : public FPromise
	{
	public:
		// [필수] Task 생성 로직
		TCoroTask<T> get_return_object()
		{
			auto NewContext = MakeShared<TCoroContext<T>, ESPMode::ThreadSafe>();
			Context = NewContext;
			Context->SetPromise(this);
			SetCoroutineHandle(std::coroutine_handle<TPromise>::from_promise(*this));
			return TCoroTask<T>(NewContext);
		}
		
		// [필수] return_void 처리 로직
		void return_void() { Context->MarkCompleted(); }
	};
}