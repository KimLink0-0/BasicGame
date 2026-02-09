// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorExtensionWorldSubsystem.h"

#include "CommonGame/Experience/ExperienceManagerComponent.h"
#include "CommonGame/FrameworkActors/CommonCharacter.h"
#include "CommonGame/FrameworkActors/CommonGameModeBase.h"
#include "CommonGame/FrameworkActors/CommonGameStateBase.h"
#include "CommonGame/FrameworkActors/CommonPlayerController.h"
#include "CommonGame/FrameworkActors/CommonPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"

void UActorExtensionWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// GameFrameworkComponentManager 에 ExtensionHandler 등록
	// Actor 의 구성요소가 완성되는 시점에서 실행되는 델리게이트
	RegisterWithGameFrameworkComponentManager();
	
	// Experience 로드 대기하는 코루틴
	WaitForExperienceLoadedCoroutine();
}

void UActorExtensionWorldSubsystem::Deinitialize()
{
	// 1. Extension 작업 완료된 Actor 들을 순회하며
	for (auto& Pair : CompleteActors)
	{
		AActor* Actor = Pair.Key.Get();
		if (!Actor)
		{
			continue;
		}
		
		// 2. Extension 을 비활성화하고
		for (FActorExtension& Extension : Pair.Value.Extensions)
		{
			if (Extension.IsActivated())
			{
				Extension.OnDeactivate(Actor);
			}
		}
	}
	
	// 3. 남은 데이터를 정리한다
	UncheckedActors.Empty();
	RegisterActors.Empty();
	CompleteActors.Empty();
	ClassExtensionCache.Empty();
	ExtensionHandles.Empty();
	
	Super::Deinitialize();
}

void UActorExtensionWorldSubsystem::Tick(float DeltaTime)
{
	// Tick 을 활용하여 조건 체크 및 누락 부분 재실행
	
	// -------------------------------------------------------------------
	// 1. 미확인목록 확인 진행
	// UncheckedActors -> RegisterActors 로 이동
	// -------------------------------------------------------------------
	for (int32 i = UncheckedActors.Num() - 1; i >= 0; --i)
	{
		// Actor 가 유효하지 않으면 목록에서 제거 후 다음 대상 작업 진행
		AActor* Actor = UncheckedActors[i].Get();
		if (!Actor)
		{
			UncheckedActors.RemoveAtSwap(i);
			continue;
		}
		
		// Extension 진행 대상인지 확인 작업
		TArray<FActorExtension> Extensions;
		if (TryCreateExtensions(Actor, Extensions))
		{
			// UncheckedActors -> RegisterActors 로 이동
			RegisterActors.Add(FRegisterActorData{Actor, MoveTemp(Extensions)});
		}
		
		// 확인 했으니 미확인목록에서 제거
		UncheckedActors.RemoveAtSwap(i);
	}
	
	// -------------------------------------------------------------------
	// 2. 등록된목록 활성화 진행
	// RegisterActors -> CompleteActors
	// -------------------------------------------------------------------
	for (int32 i = RegisterActors.Num() - 1; i >= 0; --i)
	{
		FRegisterActorData& Data = RegisterActors[i];
		AActor* Actor = Data.Actor.Get();
		
		// Actor 가 유효하지 않으면 목록에서 제거 후 다음 대상 작업 진행
		if (!Actor)
		{
			RegisterActors.RemoveAtSwap(i);
			continue;
		}
		
		// Extension 을 활성화 진행하면서 모든 Extension 이 활성화 되었는 지 확인
		bool bAllActivated = true;
		for (FActorExtension& Extension : Data.Extensions)
		{
			if (!Extension.IsActivated())
			{
				if (Extension.CanActivate(Actor))
				{
					Extension.OnActivate(Actor);
				}
				else
				{
					bAllActivated = false;
				}
			}
		}
		
		// RegisterActors -> CompleteActors 로 이동
		if (bAllActivated)
		{
			CompleteActors.Add(Actor, FCompleteActorData{MoveTemp(Data.Extensions)});
			RegisterActors.RemoveAtSwap(i);
		}
	}
}

TStatId UActorExtensionWorldSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UActorExtensionWorldSubsystem, STATGROUP_Tickables);
}

bool UActorExtensionWorldSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	// Game 과 PIE 에서만 동작
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UActorExtensionWorldSubsystem::IsTickable() const
{
	return (UncheckedActors.Num() + RegisterActors.Num() > 0) && bExperienceLoaded;
}

void UActorExtensionWorldSubsystem::RegisterExtensionForClass(UClass* TargetClass, const FActorExtension& Extension, bool bAddToLocallyControlled, bool bAddToSimulatedProxy)
{
	if (!TargetClass)
	{
		return;
	}
	
	TArray<UClass*> DerivedClasses;
	GetDerivedClasses(TargetClass, DerivedClasses, true);
	DerivedClasses.Add(TargetClass);
	
	for (UClass* Class : DerivedClasses)
	{
		FClassExtensionMapping& Mapping = ClassExtensionCache.FindOrAdd(Class);
		Mapping.Entries.Add(FExtensionEntry{Extension, bAddToLocallyControlled, bAddToSimulatedProxy});
	}
}

void UActorExtensionWorldSubsystem::UnregisterExtensionsForClass(UClass* TargetClass)
{
	if (!TargetClass)
	{
		return;
	}
	
	TArray<UClass*> DerivedClasses;
	GetDerivedClasses(TargetClass, DerivedClasses, true);
	DerivedClasses.Add(TargetClass);

	for (UClass* Class : DerivedClasses)
	{
		ClassExtensionCache.Remove(Class);
	}
}

bool UActorExtensionWorldSubsystem::IsActorComplete(const AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}
	
	return CompleteActors.Contains(Actor);
}

TCoroTask<void> UActorExtensionWorldSubsystem::WaitForExperienceLoadedCoroutine()
{
	co_await UExperienceManagerComponent::WaitForExperienceLoadedStaticCoroutine(GetWorld());
	bExperienceLoaded = true;
}

void UActorExtensionWorldSubsystem::RegisterWithGameFrameworkComponentManager()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return;
	}
	
	UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
	if (!GFCM)
	{
		return;
	}
	
	const TArray TargetClasses = {
		ACommonGameModeBase::StaticClass(),
		ACommonGameStateBase::StaticClass(),
		ACommonPlayerController::StaticClass(),
		ACommonPlayerState::StaticClass(),
		ACommonCharacter::StaticClass()
	};
	
	// FExtensionHandlerDelegate: 
	// 1. Actor 의 구성 요소가 준비되는 시점에서 실행 (World 에 Spawn 될 때, ModularActors 의 BeginPlay() 에서 실행)
	// 2. 레벨 이동 및 PIE 추가 시 실행
	// 3. 런타임에 Component 가 추가 될 때 실행 
	auto Delegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtensionEvent);
	
	for (UClass* TargetClass : TargetClasses)
	{
		ExtensionHandles.Add(GFCM->AddExtensionHandler(TargetClass, Delegate));
	}
}

void UActorExtensionWorldSubsystem::HandleActorExtensionEvent(AActor* Actor, FName EventName)
{
	if (!Actor)
	{
		return;
	}
	
	// Actor 가 준비되어 있다면, UncheckedActors 에 추가 (ModularActors 의 BeginPlay 시점에서 추가)
	if (EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
	{
		UncheckedActors.Add(Actor);
	}
	// Actor 가 Receiver 에서 제거되어 있다면, Extension 관리 대상에서 제거 작업 진행
	else if (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveActor(Actor);
	}
}

bool UActorExtensionWorldSubsystem::TryCreateExtensions(AActor* Actor, TArray<FActorExtension>& OutExtensions)
{
	if (!Actor)
	{
		return false;
	}
	
	const FClassExtensionMapping* Mapping = ClassExtensionCache.Find(Actor->GetClass());
	if (!Mapping || Mapping->Entries.Num() == 0)
	{
		return false;
	}
	
	// 필터링 작업
	const bool bIsLocallyControlled = Actor->HasLocalNetOwner();
	const bool bIsSimulatedProxy = (Actor->GetLocalRole() == ROLE_SimulatedProxy);
	
	for (const FExtensionEntry& Entry : Mapping->Entries)
	{
		// 1. LocalController 확인
		// Entry 에서 내가 조종하는 것에 추가할 것인지 Action 에서 조건을 설정
		// Extension 에서는 해당 Extension 의 상태를 표시
		// 조건에 상태가 부합해야만 실행
		if (bIsLocallyControlled && !Entry.bAddToLocallyControlled)
		{
			continue;
		}
		
		// 2. NetRole 확인
		// 위와 동일
		if (bIsSimulatedProxy && !Entry.bAddToSimulatedProxy)
		{
			continue;
		}
		
		// 3. Local 이면서도 Simulated 가 아니라면 FActorExtension 에 추가 
		OutExtensions.Add(Entry.Extension);
	}
	
	return OutExtensions.Num() > 0;
}

void UActorExtensionWorldSubsystem::RemoveActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}
	
	// CompleteActors 에서 제거 및 Extension 비활성화 호출
	if (FCompleteActorData* Data = CompleteActors.Find(Actor))
	{
		for (FActorExtension& Extension : Data->Extensions)
		{
			if (Extension.IsActivated())
			{
				Extension.OnDeactivate(Actor);
			}
		}
		
		CompleteActors.Remove(Actor);
		return;
	}
	
	// RegisterActors 에서 제거
	for (int32 i = RegisterActors.Num() - 1; i >= 0; --i)
	{
		if (RegisterActors[i].Actor.Get() == Actor)
		{
			RegisterActors.RemoveAtSwap(i);
			return;
		}
	}
	
	// UncheckedActors 에서 제거
	for (int32 i = UncheckedActors.Num() - 1; i >= 0; --i)
	{
		if (UncheckedActors[i] == Actor)
		{
			UncheckedActors.RemoveAtSwap(i);
			return;
		}
	}
}
