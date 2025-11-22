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

// 매크로 대신에 싱글톤 형식으로 사용하기
// 에셋매니저 클래스를 선언한후 사용해야 한다.
// 
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
	/*Primary*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Dexterity);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);

	/*Secondary*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Block);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_StaminaRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxStamina);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Shift);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Player_Staff);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Player_Bow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_Spear);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_Slingshot);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_ShamanStaff);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);


	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);

	extern TArray<FGameplayTag> DamageTypes;
	
}
