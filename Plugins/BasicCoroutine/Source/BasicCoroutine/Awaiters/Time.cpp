#include "Time.h"

namespace Coro::Latent
{
	// SecondsAwaiter's Section
	FSecondsAwaiter::FSecondsAwaiter(UObject* InOwner, double InDuration) :
		TLatentAwaiterBase(InOwner),
		TargetDuration(InDuration + FPlatformTime::Seconds())
	{
	}

	bool FSecondsAwaiter::ShouldResume()
	{
		// 현재 시간이 타겟 시간을 넘어가면 true
		return FPlatformTime::Seconds() >= TargetDuration;
	}
	
	// FramesAwaiter's Section
	FFramesAwaiter::FFramesAwaiter(UObject* InOwner, int32 InFrame) :
		TLatentAwaiterBase(InOwner),
		TargetFrame(GFrameCounter + FMath::Max(InFrame, 1))
	{
	}
	
	bool FFramesAwaiter::ShouldResume()
	{
		// 현재 프레임 카운터가 타겟 프레임을 넘어가면 true
		return GFrameCounter >= TargetFrame;
	}
	
	// UntilAwaiter's Section
	FUntilAwaiter::FUntilAwaiter(UObject* InOwner, TFunction<bool()> InPredicate) :
		TLatentAwaiterBase(InOwner),
		Predicate(MoveTemp(InPredicate))
	{
	}
	
	bool FUntilAwaiter::ShouldResume()
	{
		// 함수가 유효하고, 함수의 결과가 true 면 true
		return Predicate && Predicate();
	}
}
