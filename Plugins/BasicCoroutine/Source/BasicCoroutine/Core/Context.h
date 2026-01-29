#pragma once

#include "CoreMinimal.h"
#include <atomic> // 멀티 스레드 환경에서 안전하게 변수를 수정하기 위해 필요

// 전방 선언 용
namespace Coro::Private
{
	class FPromise;
}

// 코루틴의 현재 상태를 나타내는 열거형
enum class ECoroState : uint8
{
	Pending,		// 대기 중 
	Running,		// 실행 중
	Completed,		// 정상 완료 (co_return 을 확인)
	Canceled,		// 취소됨 (강제로 종료됨)
};

// [Context 의 역할] 코루틴의 상태를 저장 및 공유
// Task(사용자) 와 Promise(시스템) 가 해당 객체를 shared_ptr 로 같이 사용
struct FCoroContext
{
public:
	FCoroContext() = default;
	
	// 복사 생성 금지: Context 는 유일해야 하므로 복사 생성을 막는다.
	FCoroContext(const FCoroContext&) = delete;
	FCoroContext& operator=(const FCoroContext&) = delete;
	
	// 상태 조회 (Read)
	
		// 코루틴 종료 확인: Completed 와 Canceled 를 구분하지 않고 true 반환
		bool IsDone() const
		{
			// atomic load: 다른 스레드가 수정 중일 때 읽지 않도록 설정
			ECoroState CurrentState = State.load(std::memory_order_relaxed);
			return CurrentState == ECoroState::Completed || CurrentState == ECoroState::Canceled;
		}
		
		// 현재 상태 가져오기
		ECoroState GetState() const
		{
			return State.load(std::memory_order_relaxed);
		}
	
	// 상태 변경 (Write)
		
		// 완료 처리: Promise 가 작업을 완료하면 호출
		void MarkCompleted()
		{
			State.store(ECoroState::Completed, std::memory_order_relaxed);
		}
	
		// 취소 처리: 사용자가 Task.Cancel() 을 호출하면 실행
		void Cancel()
		{
			// Compare-And-Swap 연산
			// 만약 현재 상태가 Pending 이면, Canceled 로 변경
			ECoroState Expected = ECoroState::Pending;
			if (!State.compare_exchange_strong(Expected, ECoroState::Canceled))
			{
				// Pending 이 아니었다면
				// Running 인지 확인하고 취소
				Expected = ECoroState::Running;
				State.compare_exchange_strong(Expected, ECoroState::Canceled);
			}
		}
	
	// 내부 연결
	
		// Promise 포인터 설정
		// 코루틴이 실행되는 동안 Promise 의 위치를 기억
		void SetPromise(Coro::Private::FPromise* InPromise)
		{
			Promise = InPromise;
		}
	
private:
	// 원자적 상태 변수 (Thread-Safe)
	std::atomic<ECoroState> State {ECoroState::Pending};
	
	// 실행 중인 Promise 포인터
	// Promise 는 코루틴이 끝나면 사라지므로, 사용 시 유효성 체크 필수
	Coro::Private::FPromise* Promise = nullptr;
};