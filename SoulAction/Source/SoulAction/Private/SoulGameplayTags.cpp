// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameplayTags.h"

//FSoulGameplayTags FSoulGameplayTags::GameplayTags;
//
//void FSoulGameplayTags::InitializeNativeGameplayTags()
//{
//	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces dagame taken, improves block chance"));
//}


namespace SoulGameplayTags
{
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Strength, "Attributes.Primary.Strength", "Strength");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Dexterity, "Attributes.Primary.Dexterity", "Dexterity");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Primary_Vigor, "Attributes.Primary.Vigor", "Vigor");


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces dagame taken, improves block chance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration", "ArmorPenetration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Block, "Attributes.Secondary.Block", "BlockChance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "CriticalHitChance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Increase Criticla Hit Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "HealthRegeneration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_StaminaRegeneration, "Attributes.Secondary.StaminaRegeneration", "StaminaRegeneration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxStamina, "Attributes.Secondary.MaxStamina", "MaxStamina");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "InputTag LMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "InputTag RMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Shift, "InputTag.Shift", "InputTag Shift");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Player_Staff, "WeaponTag.Player.Staff", "WeaponTag Staff");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Player_Bow, "WeaponTag.Player.Bow", "WeaponTag Bow");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_Spear, "WeaponTag.Enemy.Spear", "WeaponTag Spear");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_Slingshot, "WeaponTag.Enemy.Slingshot", "WeaponTag Slingshot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_ShamanStaff, "WeaponTag.Enemy.ShamanStaff", "WeaponTag ShamanStaff");

	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Damage", "Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", "Fire Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Physical Damage Type");
	
	TArray<FGameplayTag> DamageTypes = 
	{
		Damage_Fire,
		Damage_Physical
	};

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Weapon, "CombatSocket.Weapon", "Weapon CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_RightHand, "CombatSocket.RightHand", "RightHand CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_LeftHand, "CombatSocket.LeftHand", "LeftHand CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Tail, "CombatSocket.Tail", "Tail CombatSocket Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_1, "Montage.Attack.1", "Montage_Attack_1 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_2, "Montage.Attack.2", "Montage_Attack_2 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_3, "Montage.Attack.3", "Montage_Attack_3 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_4, "Montage.Attack.4", "Montage_Attack_4 Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Summon, "Abilities.Summon", "Summon Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effects_HitReact, "Effects.HitReact", "HitReact");
}

