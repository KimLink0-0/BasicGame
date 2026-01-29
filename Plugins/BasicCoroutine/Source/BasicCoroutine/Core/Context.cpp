#include "Context.h"

#include "Promise.h"

void FCoroContext::Resume() const
{
	if (Promise)
	{
		Promise->Resume();
	}
}

void FCoroContext::SetPromise(Coro::Private::FPromise* InPromise)
{
	Promise = InPromise;
}
