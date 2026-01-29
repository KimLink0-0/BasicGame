// Fill out your copyright notice in the Description page of Project Settings.


#include "CoroutineTestActor.h"

#include "BasicGameLogChannel.h"

ACoroutineTestActor::ACoroutineTestActor()
{
}

TCoroTask<void> ACoroutineTestActor::FirstTestCoroutine()
{
	UE_LOG(LogBG, Log, TEXT("Hello Coroutine!"));
	co_return;
}

// Called when the game starts or when spawned
void ACoroutineTestActor::BeginPlay()
{
	Super::BeginPlay();
	FirstTestCoroutine();
}

