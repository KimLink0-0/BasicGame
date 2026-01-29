#pragma once

#include "Context.h"

// [Task 의 역할] 사용자가 코루틴을 제어하는 핸들(Handle)
template<typename T>
class TCoroTask
{
public:
	// 기본 생성자
	TCoroTask() = default;
	
	// 생성자: Promise 가 만든 Context 를 받아서 저장
	TCoroTask(TSharedPtr<FCoroContext> InContext) : 
		Context(InContext)
	{
	}
	
	// 사용자 편의 함수
		
		// 코루틴이 유효한지 확인
		bool IsValid() const { return Context.IsValid(); }
		
		// 코루틴이 끝났는지 확인
		bool IsDone() const
		{
			return Context.IsValid() && Context->IsDone();
		}
	
		// 코루틴 취소 요청
		void Cancel()
		{
			if (Context.IsValid())
			{
				Context->Cancel();
			}
		}
	
private:
	// Context 는 Task 와 Promise 가 공유
	// Promise 가 파괴되어도 Task 가 살아있으면 Context 는 유지
	TSharedPtr<FCoroContext> Context;
};