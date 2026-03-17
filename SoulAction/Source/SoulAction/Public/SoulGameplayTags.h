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

	/*Meta*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta_IncomingXP);

	/*InputTag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Shift);

	/*WeaponTag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Player_Staff);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Player_Bow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Player_Sword);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_Spear);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_Slingshot);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponTag_Enemy_ShamanStaff);

	/*Damage Tag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);
	/*Debuff*/

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Params_Chance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Params_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Params_Durtion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Params_Frequency);


	/*SocketTag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail);

	/*Attack MontageTag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

	/*Ability Tag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_None);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_HitReact);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Offensive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_Passive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Type_None);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Player_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire_FireBolt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning_Electrocute);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Sword_MeleeAttack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Rolling);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Knockback);
	/*CooldownTag*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Fire_FireBolt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Player_Dash);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);


	/*입력 차단시 사요*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_UI_Open);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputPressed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputHeld);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_InputReleased);

	/*CueDebuff에서 사용할 태그들*/
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Stun);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Debuff_Bleeding);

	extern TArray<FGameplayTag> DamageTypes;
	extern TMap<FGameplayTag,FGameplayTag> DamageTypesToDebuffs;
	extern TMap<FGameplayTag, FGameplayTag> DebuffTypeToGameplayCue;
}
