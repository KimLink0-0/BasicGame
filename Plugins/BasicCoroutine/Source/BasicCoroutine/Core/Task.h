#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include "BasicCoroutine/Core/Context.h"

template<>
class TCoroTask<void>
{
public:
	using FResultType = void;
	
	TCoroTask() = default;
	
	explicit TCoroTask(FCoroContextPtr InContext) : 
		Context(InContext) 
	{
	}
	
	// // 복사 생성 가능
	// TCoroTask(const TCoroTask&) = default;
	// TCoroTask& operator=(const TCoroTask&) = default;
	//
	// // 데이터 이동 가능
	// TCoroTask(TCoroTask&&) = default;
	// TCoroTask& operator=(TCoroTask&&) = default;
	
	// 상태 읽기
	
	bool IsValid() const { return Context != nullptr; }
	
	bool IsDone() const { return Context && Context->IsDone(); }
	
	// bool WasSuccessful() const { return Context.IsValid() /**&& Context->WasSuccessful()**/; }
	
	// 상태 쓰기
	// void Cancel()
	// {
	// 	if (Context)
	// 	{
	// 		Context->Cancel();
	// 	}
	// }
	//
	// bool Wait(uint32 TimeoutMs = MAX_uint32) const
	// {
	// 	return Context.IsValid() /** && Context->Wait(TimeoutMS) **/;
	// }
	
	void ContinueWith(TFunction<void()> Callback) const
	{
		if (Context)
		{
			Context->AddCompletionCallback(MoveTemp(Callback));
		}
	}
	
protected:
	Coro::Private::FCoroContext* GetContext() const { return Context.Get(); }
	
	template<typename T>
	TCoroContextPtr<T> GetSharedContext() const
	{
		return StaticCastSharedPtr<Coro::Private::TCoroContext<T>>(Context);
	}
	
protected:
	// Member Variables Section
	FCoroContextPtr Context;
};

template<typename T>
class TCoroTask : public TCoroTask<void>
{
public:
	using FResultType = T;
	
	using TCoroTask<void>::ContinueWith;
	
	TCoroTask() = default;
	
	explicit TCoroTask(TCoroContextPtr<T> InContext) : 
		TCoroTask<void>(InContext)
	{
	}
	
	// const T& GetResult() const
	// {
	// 	check(IsDone());
	// 	return this->template GetSharedContext<T>()->GetResult();
	// }
	//
	// T&& MoveResult()
	// {
	// 	check(IsDone())
	// 	return this->template GetSharedContext<T>()->MoveResult();
	// }
	
	void ContinueWith(TFunction<void(const T&)> Callback) const
	{
		if (Context)
		{
			TCoroContextWeakPtr<T> WeakContext = this->template GetSharedContext<T>();
			
			Context->AddCompletionCallback([WeakContext, Callback = MoveTemp(Callback)]()
			{
				if (TCoroContextPtr<T> StrongContext = WeakContext.Pin())
				{
					Callback(StrongContext->GetResult());
				}
			});
		}
	}
};