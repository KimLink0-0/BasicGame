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
	
	struct BASICCOROUTINE_API FCoroContext
	{
	public:
		// 생성자
		FCoroContext() = default;
		virtual ~FCoroContext() = default;
		
		// 복사 금지
		FCoroContext(const FCoroContext&) = delete;
		FCoroContext& operator=(const FCoroContext&) = delete;
		
		// 상태 조회 (Read)
		bool IsDone() const;
		
		// 상태 변경 (Write)
		void MarkCompleted();
		
		void Cancel();
		
		void Resume() const;
		
		void SetPromise(FPromise* InPromise);
		
	protected:
		// 기본 상태: 대기 중
		std::atomic<ECoroState> State = ECoroState::Pending;
		
		FPromise* Promise = nullptr;
		
		// 동기화 용
		mutable FCriticalSection Lock;
	};
	
	// 코루틴의 결과 값을 담는 템플릿 
	template<typename T>
	struct TCoroContext : public FCoroContext
	{
	public:
		void SetResult(T&& InResult)
		{
			Result = MoveTemp(InResult);
		}
		
		const T& GetResult() const
		{
			return Result;
		}
		
		T&& MoveResult()
		{
			return MoveTemp(Result);
		}
		
	private:
		T Result{};
	};
	
	// 코루틴의 결과 값이 void 인 템플릿
	template<>
	struct TCoroContext<void> : public FCoroContext
	{
		
	};
}
