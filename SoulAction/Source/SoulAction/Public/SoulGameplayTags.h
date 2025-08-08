// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h" 

/**
 *  GameplayTag를 구조체를 사용해서 만들기
 *  나중에 UE_DECLARE_GAMEPLAY_TAG_EXTERN를 사용해서 만드는걸로 바꿔보기
 *  예시) .h파일 UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListFleets);
 *  .cpp 파일 UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServersTag.GameSessionsAPI.ListFleets", "List Fleets resource on the GameSessions API")
 */

//struct FSoulGameplayTags
//{
//public:
//	static const FSoulGameplayTags& Get() { return GameplayTags; }
//	static void InitializeNativeGameplayTags();
//
//protected:
//
//private:
//	static FSoulGameplayTags GameplayTags;
//};


namespace SoulGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
}
