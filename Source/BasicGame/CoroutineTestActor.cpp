// Fill out your copyright notice in the Description page of Project Settings.


#include "CoroutineTestActor.h"

#include "BasicGameLogChannel.h"
#include "BasicCoroutine/Awaiters/TestAwaiter.h"

ACoroutineTestActor::ACoroutineTestActor()
{
}

TCoroTask<void> ACoroutineTestActor::FirstTestCoroutine()
{
	UE_LOG(LogBG, Log, TEXT("1. Start Coroutine"));
	
	co_await Coro::Test::WaitNextTick();
	
	UE_LOG(LogBG, Log, TEXT("2. Resume Coroutine"))
	
	co_return;
}

// Called when the game starts or when spawned
void ACoroutineTestActor::BeginPlay()
{
	Super::BeginPlay();
	FirstTestCoroutine();
}

