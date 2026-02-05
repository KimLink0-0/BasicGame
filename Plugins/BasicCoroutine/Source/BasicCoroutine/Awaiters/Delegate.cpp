// Fill out your copyright notice in the Description page of Project Settings.


#include "Delegate.h"

namespace  Coro::Private
{
	FSimpleDelegateAwaiter::FSimpleDelegateAwaiter(UObject* InOwner, FSimpleMulticastDelegate& InDelegate) :
		Super(InOwner),
		Delegate(&InDelegate)
	{
	}

	FSimpleDelegateAwaiter::~FSimpleDelegateAwaiter()
	{
		if (DelegateHandle.IsValid())
		{
			Delegate->Remove(DelegateHandle);
		}
	}

	void FSimpleDelegateAwaiter::Suspend()
	{
		DelegateHandle = Delegate->AddLambda([CapturedOwner = Owner, CapturedContext = Context, DelegatePtr = Delegate, DelegateHandlePtr = &DelegateHandle]()
		{
			if (CapturedOwner.IsValid())
			{
				DelegatePtr->Remove(*DelegateHandlePtr);
				DelegateHandlePtr->Reset();
			}
			SafeResume(CapturedOwner, CapturedContext);
		});
	}
}
