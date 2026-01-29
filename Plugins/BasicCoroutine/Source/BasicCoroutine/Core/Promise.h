#pragma once

#include "CoreMinimal.h"
#include <coroutine>
#include "Task.h"
#include "Context.h"

namespace Coro::Private
{
	// [FPromise 의 역할] 모든 Promise 의 공통 부모 클래스
	class FPromise
	{
	public:
		FPromise() = default;
		
		virtual ~FPromise()
		{
			if (Context.IsValid())
			{
				// 중요: 댕글링 포인터 방지
				// Promise 가 파괴될 때 Context 에 "나 나간다" 라고 알리는 일
				Context->SetPromise(nullptr);
			}
		}
		
		// co_await 변환 함수
		// 코루틴 안에서 co_await X; 를 호출하면 -> await_transform(X) 가 호출
		template<typename T>
		decltype(auto) await_transform(T&& Awaitable)
		{
			// 들어온 Awaiter 를 그대로 반환
			// 나중에 분기처리 로직이 들어올 예정
			return std::forward<T>(Awaitable);
		}
		
		void SetCoroutineHandle(std::coroutine_handle<> Handle)
		{
			CoroutineHandle = Handle;
		}
		
		void Resume()
		{
			if (CoroutineHandle)
			{
				CoroutineHandle.resume();
			}
		}
		
		// 코루틴 표준 인터페이스
		
		// [필수 1.] 코루틴 시작 시 대기 여부
		// suspend_never 로 설정하면 바로 실행
		std::suspend_never initial_suspend() const noexcept { return {}; }
		
		// [필수 2.] 코루틴 종료 시 대기 여부
		// suspend_never 로 설정하면 즉시 종료, awaiter 를 구현하면 suspend_always 를 사용하는 것도 구현 가능
		std::suspend_never final_suspend() const noexcept { return {}; }
		
		// [필수 3.] 예외 처리
		// 언리얼에서는 예외를 미사용 -> 강제 종료
		void unhandled_exception() const noexcept { check(false); }
		
	protected:
		// Task 과 공유하는 Context 포인터
		TSharedPtr<FCoroContext> Context;
		
		// 코루틴 핸들 저장
		std::coroutine_handle<> CoroutineHandle;
	};
		
	// [TCoroPromise 의 역할] 리턴 타입 T 에 특화
	template<typename T>
	class TCoroPromise : public FPromise
	{
	public:
		// [필수 4.] 코루틴 객체 (Task) 생성
		TCoroTask<T> get_return_object()
		{
			// 1. Context 생성 (ThreadSafe 모드)
			Context = MakeShared<FCoroContext, ESPMode::ThreadSafe>();
			
			// 2. Context 와 Promise 연결
			Context->SetPromise(this);
			
			// 자신 (TCoroPromise) 의 Handle 을 저장 
			SetCoroutineHandle(std::coroutine_handle<TCoroPromise>::from_promise(*this));
			
			// 3. Task 에 Context 를 담아서 반환
			return TCoroTask<T>(Context);
		}
		
		// [필수 5.] co_return; 처리 (void 버전)
		void return_void()
		{
			if (Context.IsValid())
			{
				// Context 상태를 '완료' 로 변경
				Context->MarkCompleted();
			}
		}
	};
}