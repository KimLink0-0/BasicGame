#pragma once

#include "BasicCoroutine/Core/Awaiter.h"

namespace Coro::Test
{
	struct FTestAwaiter : public Coro::Private::FAwaiterBase
	{
		
	};
	
	// 사용자가 호출할 함수
	inline FTestAwaiter WaitNextTick()
	{
		return FTestAwaiter();
	}
}