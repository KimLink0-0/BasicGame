#pragma once

#include <coroutine>
#include "CoreMinimal.h"

namespace Coro::Private
{
	// [Awaiter 의 역할] 모든 Awaiter 의 공통 부모 클래스
	// 상속 받은 클래스는 await_suspend 만 구현하면 됨
	struct FAwaiterBase
	{
		// 1. 준비가 되었는지 확인
		// false 를 리턴하면, 아직 준비되지 않았으니 멈추라는 뜻
		// true 를 리턴하면, 멈추지 않고 바로 await_resume 으로 넘어감
		bool await_ready() const { return false; }
		
		// 2. 멈췄을 때 할 동작
		// Handle: 멈춘 코루틴을 재개하게 만들 수 있는 핸들
		template<typename PromiseType>
		void await_suspend(std::coroutine_handle<PromiseType> Handle)
		{
			// 할 작업이 없으니 임시로 바로 재개
			Handle.resume();
		}
		
		// 3. 다시 시작할 때 결과값 반환 여부
		// void 면 결과값 없음
		void await_resume() {}
	};
}
