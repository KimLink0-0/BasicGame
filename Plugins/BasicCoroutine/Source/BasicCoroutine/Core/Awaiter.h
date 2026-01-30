#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Private.h"
#include "BasicCoroutine/Core/Promise.h"
#include "LatentAction.h"

namespace Coro::Private
{
	// Template Param, Derived : 파생 클래스 타입
	// Base 함수에는 없는 파생 클래스의 함수 호출을 하는데 사용할 수 있음
	template<typename Derived>
	class TLatentAwaiterBase : public ILatentPollable
	{
	public:
		explicit TLatentAwaiterBase(UObject* InOwner) : Owner(InOwner) {}
		
		// 일단 정지 
		bool await_ready() { return false; }
		
		// 멈춘 뒤 실행할 동작 설정
		template<typename Promise>
		void await_suspend(std::coroutine_handle<Promise> Handle)
		{
			FCoroContextPtr Context = Handle.promise().GetContextForAwaiter();
			
			if (!Owner.IsValid())
			{
				Context->Resume();
				return;
			}
			
			UWorld* World = GetWorld();
			if (!World)
			{
				Context->Resume();
				return;
			}
			
			// LatentManager 에 LatentAction 을 등록
			FLatentActionManager& LatentManager = World->GetLatentActionManager();
			LatentManager.AddNewAction(Owner.Get(), GetLatentUUID(), new FCoroLatentAction(this, Context));
		}
		
		void await_resume()
		{
			// LatentActionBase 를 상속받는 클래스에서 GetResult() 가 있다면 실행
			if constexpr (requires { static_cast<Derived*>(this)->GetResult(); })
			{
				return static_cast<Derived*>(this)->GetResult();
			}
		}
		
	protected:
		UObject* GetOwner() const { return Owner.Get(); }
		
		UWorld* GetWorld() const { return Owner.IsValid() ? Owner->GetWorld() : nullptr; }
		
		// 메모리 주소 기반, 고유 ID 값을 생성하는 방법 
		// 1. 메모리 주소를 가져와서
		// 2. 포인터를 정수로 변환
		// 3. 64비트 정수를 다시 32비트 정수로 변환
		int32 GetLatentUUID() const { return static_cast<int32>(reinterpret_cast<UPTRINT>(static_cast<const Derived*>(this))); }
		
	private:
		TWeakObjectPtr<UObject> Owner;
	};
}