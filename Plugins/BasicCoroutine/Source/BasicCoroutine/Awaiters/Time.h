#pragma once

#include "CoreMinimal.h"
#include "BasicCoroutine/Core/Awaiter.h"

namespace Coro::Latent
{
	// FSeconds's Section
		class BASICCOROUTINE_API FSecondsAwaiter : public Private::TLatentAwaiterBase<FSecondsAwaiter>
		{
		public:
			FSecondsAwaiter(UObject* InOwner, double InDuration);
			
			virtual bool ShouldResume() override;
			
		private:
			double TargetDuration;
		};
	
		// 객체 없이 호출하기 위해서 class 밖에서 선언
		inline FSecondsAwaiter Seconds(UObject* InOwner, double InDuration) { return FSecondsAwaiter(InOwner, InDuration);}
		
		
	// Frames's Section
		class BASICCOROUTINE_API FFramesAwaiter : public Private::TLatentAwaiterBase<FFramesAwaiter>
		{
		public:
			FFramesAwaiter(UObject* InOwner, int32 InFrame);
			
			virtual bool ShouldResume() override;
		
		private:
			uint64 TargetFrame;
		};
		
		inline FFramesAwaiter Frames(UObject* InOwner, int32 InFrame) { return FFramesAwaiter(InOwner, InFrame); }
			
		inline FFramesAwaiter NextTick(UObject* InOwner) { return FFramesAwaiter(InOwner, 1); }
	
	
	// Until's Section
		class BASICCOROUTINE_API FUntilAwaiter : public Private::TLatentAwaiterBase<FUntilAwaiter>
		{
		public:
			// Predicate: 수학적으로 주어진 조건을 만족하는지 아닌지를 테스트하는 함수나 표현식
			FUntilAwaiter(UObject* InOwner, TFunction<bool()> InPredicate);
			
			virtual bool ShouldResume() override;
			
		private:
			TFunction<bool()> Predicate;
		};
		
		inline FUntilAwaiter Until(UObject* InOwner, TFunction<bool()> InPredicate) { return FUntilAwaiter(InOwner, MoveTemp(InPredicate)); }
}