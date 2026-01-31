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
			// Context 가 생성될 때 Promise 의 포인터를 넘기기 때문에
			// Promise 가 파괴될 때 Context 에 있는 Promise 의 포인터 삭제
			if (Context)
			{
				Context->SetPromise(nullptr);
			}
		}
		
		// 복사 생성 금지
		FPromise(const FPromise&) = delete;
		FPromise& operator=(const FPromise&) = delete;
		
		// Context Getter
		FCoroContextPtr GetContextForAwaiter() const { return Context;}
		FCoroContext* GetContext() const { return Context.Get(); }
		
		
		/** Promise 필수 로직 **/
		
		// [필수] 시작 시 멈출 지 유무
		// suspend_never : 즉시 시작
		std::suspend_never initial_suspend() const noexcept { return {}; }
		
		// [필수] 종료 시 멈출 지 유무
		// suspend_never : 즉시 정지
		std::suspend_never final_suspend() const noexcept { return {}; }
		
		// [필수] 예외 처리 
		void unhandled_exception() const noexcept { check(false); }
		
		// co_await 을 만났을 때 처리를 위한 정의 선언
		template<typename T>
		decltype(auto) await_transform(T&& Awaitable);
		
		
		/** 코루틴 제어 함수 **/
		
		void Resume() const { CoroutineHandle.resume(); }
		
		void DestroyCoroutine()
		{
			if (CoroutineHandle)
			{
				CoroutineHandle.destroy();
				CoroutineHandle = nullptr;
			}
		}
		
	protected:
		// 코루틴 Getter, Setter 
		
		std::coroutine_handle<> GetCoroutineHandle() const { return CoroutineHandle; }
		
		void SetCoroutineHandle(std::coroutine_handle<> InHandle) { CoroutineHandle = InHandle; }
		
	protected:
		// Promise 와 Task 공유 Context
		FCoroContextPtr Context;
		
		// 코루틴 핸들
		std::coroutine_handle<> CoroutineHandle;
	};
		
	template<typename T>
	class TPromise : public FPromise
	{
	public:
		// [필수] Task 생성 로직
		TCoroTask<T> get_return_object()
		{
			Context = MakeShared<TCoroContext<T>, ESPMode::ThreadSafe>();
			Context->SetPromise(this);
			SetCoroutineHandle(std::coroutine_handle<TPromise>::from_promise(*this));
			
			return TCoroTask<T>(StaticCastSharedPtr<TCoroContext<T>>(Context));
		}
		
		// [필수] return_value 처리 로직
		void return_value(T Value)
		{
			static_cast<TCoroContext<T>*>(Context.Get())->SetValue(MoveTemp(Value));
			Context->MarkCompleted();
		}
	};
	
	template<>
	class TPromise<void> : public FPromise
	{
	public:
		// [필수] Task 생성 로직
		TCoroTask<void> get_return_object()
		{
			Context = MakeShared<TCoroContext<void>, ESPMode::ThreadSafe>();
			Context->SetPromise(this);
			SetCoroutineHandle(std::coroutine_handle<TPromise>::from_promise(*this));
			
			return TCoroTask<void>(Context);
		}
		
		// [필수] return_void 처리 로직
		void return_void()
		{
			Context->MarkCompleted();
		}
	};
}