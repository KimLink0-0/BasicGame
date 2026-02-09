// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace CommonGameplayTags
{
	//-----------------------------------------------------------------------------
	// Input.Native - 네이티브 입력 태그
	//-----------------------------------------------------------------------------

	/** 이동 입력 */
	COMMONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Native_Move);

	/** 시점 입력 */
	COMMONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Native_Look);

	//-----------------------------------------------------------------------------
	// Input.Ability - Ability 입력 태그
	//-----------------------------------------------------------------------------

	

	//-----------------------------------------------------------------------------
	// Gameplay - 게임플레이 상태 태그
	//-----------------------------------------------------------------------------

	/** Ability 입력이 차단된 상태 (스턴, 메뉴 등) */
	COMMONGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Gameplay_AbilityInputBlocked);
}
