// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameplayTags.h"

namespace CommonGameplayTags
{
	//-----------------------------------------------------------------------------
	// Input.Native - 네이티브 입력 태그
	//-----------------------------------------------------------------------------

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Native_Move, "Input.Native.Move", "이동 입력");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Native_Look, "Input.Native.Look", "시점 입력");

	//-----------------------------------------------------------------------------
	// Input.Ability - Ability 입력 태그
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// Gameplay - 게임플레이 상태 태그
	//-----------------------------------------------------------------------------

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked", "Ability 입력이 차단된 상태");
}