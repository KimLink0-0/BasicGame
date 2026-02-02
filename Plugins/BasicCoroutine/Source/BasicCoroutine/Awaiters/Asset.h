// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/Awaiter.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

namespace Coro::Private
{
	template<typename T>
	class TLoadObjectAwaiter : TAsyncAwaiterBase<TLoadObjectAwaiter<T>>
	{
		using Super = TAsyncAwaiterBase<TLoadObjectAwaiter<T>>;
		
	public:
		TLoadObjectAwaiter(UObject* InOwner, TSoftObjectPtr<T> InSoftPtr) : 
			Super(InOwner),
			SoftPtr(InSoftPtr)
		{
		}
		
		bool Ready() const
		{
			// 이미 로딩된 상태라면 Suspend() 실행하지 않음
			return SoftPtr.IsNull() || SoftPtr->Get() != nullptr;
		}
		
		void Suspend()
		{
			FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
			
			// 비동기 로딩 시작
			StreamHandle = StreamableManager.RequestAsyncLoad(
				SoftPtr.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda([CaptureOwner = this->Owner, CaptureContext = this->Context]()
				{
					SafeResume(CaptureContext);
				}
				));
		}
		
		T* GetResult() const
		{
			return SoftPtr.Get();
		}
		
	private:
		TSoftObjectPtr<T> SoftPtr;
		
		TSharedPtr<FStreamableHandle> StreamHandle;
			
	};
	
	
	template<typename T>
	class TLoadClassAwaiter : public TAsyncAwaiterBase<TLoadClassAwaiter<T>>
	{
		using Super = TAsyncAwaiterBase<TLoadClassAwaiter<T>>;
		
	public:
		TLoadClassAwaiter(UObject* InOwner, TSoftClassPtr<T> InSoftPtr) :
			Super(InOwner),
			SoftPtr(InSoftPtr)
		{
		}
		
		// 이미 로딩된 상태라면 Suspend() 실행하지 않음
		bool Ready() const
		{
			return SoftPtr.IsNull() || SoftPtr->Get() != nullptr;
		}
		
		void Suspend()
		{
			FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
			
			StreamHandle = StreamableManager.RequestAsyncLoad(
				SoftPtr.ToSoftObjectPath(),
				FStreamableDelegate::CreateLambda([CapturedOwner = this->Owner, CapturedContext = this->Context]()
				{
					SafeResume(CapturedOwner, CapturedContext);
				})
			);
		}
		
		TSubclassOf<T> GetResult() const
		{
			return SoftPtr.Get();
		}
		
	private:
		TSoftClassPtr<T> SoftPtr;
		
		TSharedPtr<FStreamableHandle> StreamHandle;
	};
	
	
	template<typename T>
	class TLoadPrimaryAssetAwaiter : public TAsyncAwaiterBase<TLoadPrimaryAssetAwaiter<T>>
	{
		using Super = TAsyncAwaiterBase<TLoadPrimaryAssetAwaiter<T>>;
		
	public:
		TLoadPrimaryAssetAwaiter(UObject* InOwner, const FPrimaryAssetId& PrimaryAssetID, TArray<FName> InBundles) :
			Super(InOwner),
			AssetId(PrimaryAssetID),
			Bundles(InBundles)
		{
		}
		
		bool Ready() const
		{
			if (!AssetId.IsValid())
			{
				return true;
			}
			
			return UAssetManager::Get().GetPrimaryAssetObject(AssetId) != nullptr;
		}
		
		void Suspend()
		{
			UAssetManager& AssetManager = UAssetManager::Get();
			
			StreamHandle = AssetManager.LoadPrimaryAsset(
				AssetId,
				Bundles,
				FStreamableDelegate::CreateLambda([CapturedOwner = this->Owner, CapturedContext = this->Context]()
					{
						SafeResume(CapturedOwner, CapturedContext);		
					})
			);
		}
		
		T* GetResult() const
		{
			return Cast<T>(UAssetManager::Get().GetPrimaryAssetObject(AssetId));
		}
		
	private:
		FPrimaryAssetId AssetId;
		
		TArray<FName> Bundles;
		
		TSharedPtr<FStreamableHandle> StreamHandle;
	};
	
	
	
class BASICCOROUTINE_API FLoadPrimaryAssetsAwaiter : public TAsyncAwaiterBase<FLoadPrimaryAssetsAwaiter>
	{
		using Super = TAsyncAwaiterBase<FLoadPrimaryAssetsAwaiter>;
		
	public:
		FLoadPrimaryAssetsAwaiter(UObject* InOwner, TArray<FPrimaryAssetId> PrimaryAssetIds, TArray<FName> InBundles);
		
		bool Ready() const;

		
		void Suspend();
		
	private:
		TArray<FPrimaryAssetId> AssetIds;
		
		TArray<FName> Bundles;
		
		TSharedPtr<FStreamableHandle> StreamHandle;
	};	
	
	
	
class BASICCOROUTINE_API FLoadGameFeatureAwaiter : public TAsyncAwaiterBase<FLoadGameFeatureAwaiter>
	{
		using Super = TAsyncAwaiterBase<FLoadGameFeatureAwaiter>;
		
	public:
		FLoadGameFeatureAwaiter(UObject* InOwner, FString InPluginURL);
		
		bool Ready() const;

		
		void Suspend();

		
		bool GetResult() const;
	
	private:
		FString PluginURL;
	
		// GetResult 는 로드 성공 여부를 입력할 것
		mutable bool bSuccess = false;
	};
	
}


namespace Coro::Async
{
	template<typename T>
	Private::TLoadObjectAwaiter<T> LoadObject(UObject* Owner, TSoftObjectPtr<T> SoftPtr)
	{
		return Private::TLoadObjectAwaiter<T>(Owner, SoftPtr);
	}
	
	template<typename T>
	Private::TLoadClassAwaiter<T> LoadClass(UObject* Owner, TSoftClassPtr<T> SoftPtr)
	{
		return Private::TLoadClassAwaiter<T>(Owner, SoftPtr);
	}
	
	template<typename T>
	Private::TLoadPrimaryAssetAwaiter<T> LoadPrimaryAsset(UObject* Owner, FPrimaryAssetId PrimaryAssetId, TArray<FName> InBundles)
	{
		return Private::TLoadPrimaryAssetAwaiter<T>(Owner, PrimaryAssetId, InBundles);
	}
	
	inline Private::FLoadPrimaryAssetsAwaiter LoadPrimaryAssets(UObject* InOwner, TArray<FPrimaryAssetId> PrimaryAssetIds, TArray<FName> InBundles)
	{
		return Private::FLoadPrimaryAssetsAwaiter(InOwner, MoveTemp(PrimaryAssetIds), MoveTemp(InBundles));
	}
	
	inline Private::FLoadGameFeatureAwaiter LoadGameFeature(UObject* InOwner, FString InPluginURL)
	{
		return Private::FLoadGameFeatureAwaiter(InOwner, MoveTemp(InPluginURL));
	}
}
