#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include <atomic>

namespace Coro::Private
{
	enum class ECoroState : uint8
	{
		Pending,	// 대기 중
		Running,	// 실행 중
		Completed,	// 정상 완료
		Canceled,	// 실행 취소
	};
	
	struct FCoroContext
	{
	public:
		// 생성자
		FCoroContext() = default;
		virtual ~FCoroContext() = default;
		
		// 복사 금지
		FCoroContext(const FCoroContext&) = delete;
		FCoroContext& operator=(const FCoroContext&) = delete;
		
		// 상태 조회 (Read)
		bool IsDone() const
		{
			ECoroState Current = State.load(std::memory_order_relaxed);
			return Current == ECoroState::Completed || Current == ECoroState::Canceled;
		}
		
		// 상태 변경 (Write)
		void MarkCompleted()
		{
			State.store(ECoroState::Completed, std::memory_order_relaxed);
		}
		
		void Cancel()
		{
			ECoroState Expected = ECoroState::Pending;
			if (!State.compare_exchange_strong(Expected, ECoroState::Canceled))
			{
				Expected = ECoroState::Running;
				State.compare_exchange_strong(Expected, ECoroState::Canceled);
			}
		}
		void Resume() const
		{
			
		}
		
	protected:
		// 기본 상태: 대기 중
		std::atomic<ECoroState> State = ECoroState::Pending;
	};
	
	// 코루틴의 결과 값을 담는 템플릿 
	template<typename T>
	struct TCoroContext : public FCoroContext
	{
		void SetResult(T&& InResult) { Result = MoveTemp(InResult); }
		T Result{};
	};
	
	// 코루틴의 결과 값이 void 인 템플릿
	template<>
	struct TCoroContext<void> : public FCoroContext {};
}