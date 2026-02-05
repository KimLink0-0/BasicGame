// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Awaiter.h"

namespace Coro::Private
{
	
	
	class BASICCOROUTINE_API FSimpleDelegateAwaiter : public TAsyncAwaiterBase<FSimpleDelegateAwaiter>
	{
		using Super = TAsyncAwaiterBase<FSimpleDelegateAwaiter>;
		
	public:
		FSimpleDelegateAwaiter(UObject* InOwner, FSimpleMulticastDelegate& InDelegate);
		~FSimpleDelegateAwaiter();
		
		void Suspend();
		
	private:
		FSimpleMulticastDelegate* Delegate;
		
		FDelegateHandle DelegateHandle;
	};
	
	
	
	template<typename... Args>
	class TDelegateAwaiter : public TAsyncAwaiterBase<TDelegateAwaiter<Args...>>
	{
		using Super = TAsyncAwaiterBase<TDelegateAwaiter<Args...>>;
		
	public:
		using FDelegateType = TMulticastDelegate<void(Args...)>;
		using FResultType = TTuple<std::decay_t<Args>...>;
		
		// 생성자 소멸자
		TDelegateAwaiter(UObject* InOwner, FDelegateType& InDelegate) :
			Super(InOwner),
			Delegate(&InDelegate)
		{
		}
		~TDelegateAwaiter()
		{
			if (DelegateHandle.IsValid())
			{
				Delegate->Remove(DelegateHandle);
			}
		}
		
		void Suspend()
		{
			DelegateHandle = Delegate->AddLambda([CapturedOwner = this->Owner, CapturedContext = this->Context, DelegatePtr = Delegate, DelegateHandlePtr = &DelegateHandle, ResultPtr = &Result](Args... InArgs)
			{
				if (CapturedOwner.IsValid())
				{
					*ResultPtr = FResultType(Forward<Args>(InArgs)...);
					DelegatePtr->Remove(*DelegateHandlePtr);
					DelegateHandlePtr->Reset();
				}
				SafeResume(CapturedOwner, CapturedContext);
			});
		}
		
		FResultType GetResult()
		{
			return MoveTemp(Result);
		}
		
		
	private:
		FDelegateType* Delegate;
		
		FDelegateHandle DelegateHandle;
		
		FResultType Result;
	};
	
	
	
	template<typename T>
	class TDelegateAwaiter<T> : public TAsyncAwaiterBase<TDelegateAwaiter<T>>
	{
		using Super = TAsyncAwaiterBase<TDelegateAwaiter<T>>;

	public:
		using FDelegateType = TMulticastDelegate<void(T)>;
		using FResultType = std::decay_t<T>;

		TDelegateAwaiter(UObject* InOwner, FDelegateType& InDelegate)
			: Super(InOwner)
			, Delegate(&InDelegate)
		{
		}

		~TDelegateAwaiter()
		{
			if (DelegateHandle.IsValid())
			{
				Delegate->Remove(DelegateHandle);
			}
		}
		
		
		void Suspend()
		{
			DelegateHandle = Delegate->AddLambda([CapturedOwner = this->Owner, CapturedContext = this->Context, DelegatePtr = Delegate, HandlePtr = &DelegateHandle, ResultPtr = &Result](T InArg)
			{
				// Owner가 유효하면 Awaiter도 유효합니다 (코루틴 프레임에 저장됨)
				if (CapturedOwner.IsValid())
				{
					*ResultPtr = Forward<T>(InArg);
					DelegatePtr->Remove(*HandlePtr);
					HandlePtr->Reset();
				}
				SafeResume(CapturedOwner, CapturedContext);
			});
		}

		FResultType GetResult()
		{
			return MoveTemp(Result);
		}

	private:
		/** 대기 중인 델리게이트 */
		FDelegateType* Delegate;

		/** 바인딩 핸들 */
		FDelegateHandle DelegateHandle;

		/** Broadcast 시 전달된 결과 */
		FResultType Result;
	};
}

namespace Coro::Async
{
	inline Private::FSimpleDelegateAwaiter WaitForDelegate(UObject* Owner, FSimpleMulticastDelegate& Delegate)
	{
		return Private::FSimpleDelegateAwaiter(Owner, Delegate);
	}
	
	template<typename... Args>
	Private::TDelegateAwaiter<Args...> WaitForDelegate(UObject* Owner, TMulticastDelegate<void(Args...)>& Delegate)
	{
		return Private::TDelegateAwaiter<Args...>(Owner, Delegate);
	}
}
