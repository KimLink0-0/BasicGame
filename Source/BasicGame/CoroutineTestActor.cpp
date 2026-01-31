// Fill out your copyright notice in the Description page of Project Settings.


#include "CoroutineTestActor.h"

#include "BasicGameLogChannel.h"
#include "BasicCoroutine/Awaiters/Time.h"


// Sets default values
ACoroutineTestActor::ACoroutineTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

TCoroTask<void> ACoroutineTestActor::TimeCoroutineTestFunction(UObject* InOwner)
{
	UE_LOG(LogCoro, Log, TEXT("코루틴 시작! 3초 대기"))
	
	co_await Coro::Latent::Seconds(InOwner, 3.0f);
	
	UE_LOG(LogCoro, Log, TEXT("코루틴 3초 대기 끝!"))
	
	co_return;
}

// Called when the game starts or when spawned
void ACoroutineTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	TimeCoroutineTestFunction(this);
}

