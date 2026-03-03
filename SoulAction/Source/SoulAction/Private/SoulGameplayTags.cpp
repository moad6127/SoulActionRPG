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

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP", "IncomingXP Meta Attriute");


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "InputTag LMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "InputTag RMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Shift, "InputTag.Shift", "InputTag Shift");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Player_Staff, "WeaponTag.Player.Staff", "WeaponTag Staff");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Player_Bow, "WeaponTag.Player.Bow", "WeaponTag Bow");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Player_Sword, "WeaponTag.Player.Sword", "WeaponTag Sword");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_Spear, "WeaponTag.Enemy.Spear", "WeaponTag Spear");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_Slingshot, "WeaponTag.Enemy.Slingshot", "WeaponTag Slingshot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Enemy_ShamanStaff, "WeaponTag.Enemy.ShamanStaff", "WeaponTag ShamanStaff");

	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Damage", "Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", "Fire Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Lightning, "Damage.Lightning", "Lightning Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Arcane, "Damage.Arcane", "Arcane Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Physical Damage Type");
	
	TArray<FGameplayTag> DamageTypes = 
	{
		Damage_Fire,
		Damage_Physical,
		Damage_Lightning,
		Damage_Arcane
	};


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Burn, "Debuff.Burn", "Debuff Burn Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Stun, "Debuff.Stun", "Debuff Stun Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Arcane, "Debuff.Arcane", "Debuff Arcane Damage Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Physical, "Debuff.Physical", "Debuff Physical Damage Type");
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs =
	{
		{Damage_Fire,Debuff_Burn},
		{Damage_Physical,Debuff_Physical},
		{Damage_Lightning,Debuff_Stun},
		{Damage_Arcane,Debuff_Arcane}
	};
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Params_Chance, "Debuff.Params.Chance", "Debuff Chance Params Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Params_Damage, "Debuff.Params.Damage", "Debuff Damage Params Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Params_Durtion, "Debuff.Params.Durtion", "Debuff Durtion Params Type");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Debuff_Params_Frequency, "Debuff.Params.Frequency", "Debuff Frequency Params Type");


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Weapon, "CombatSocket.Weapon", "Weapon CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_RightHand, "CombatSocket.RightHand", "RightHand CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_LeftHand, "CombatSocket.LeftHand", "LeftHand CombatSocket Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatSocket_Tail, "CombatSocket.Tail", "Tail CombatSocket Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_1, "Montage.Attack.1", "Montage_Attack_1 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_2, "Montage.Attack.2", "Montage_Attack_2 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_3, "Montage.Attack.3", "Montage_Attack_3 Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_4, "Montage.Attack.4", "Montage_Attack_4 Ability Tag");

	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_None, "Abilities.None", "No Ability - like nullptr for Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Attack, "Abilities.Attack", "Attack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Summon, "Abilities.Summon", "Summon Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_HitReact, "Abilities.HitReact", "HitReact Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Eligible, "Abilities.Status.Eligible", "Eligible Status Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Locked, "Abilities.Status.Locked", "Locked Status Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Equipped, "Abilities.Status.Equipped", "Equipped Status Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Status_Unlocked, "Abilities.Status.Unlocked", "Unlocked Status Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Offensive, "Abilities.Type.Offensive", "Offensive Type Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_Passive, "Abilities.Type.Passive", "Passive Type Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Type_None, "Abilities.Type.None", "None Type Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Player_Dash, "Abilities.Player.Dash", "Dash Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire_FireBolt, "Abilities.Fire.FireBolt", "FireBolt Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Sword_MeleeAttack, "Abilities.Sword.MeleeAttack", "Sword MeleeAttack Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute", "Electrocute Ability Tag");

	

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Fire_FireBolt, "Cooldown.Fire.FireBolt", "FireBolt Cooldown Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_Player_Dash, "Cooldown.Player.Dash", "Dash Cooldown Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Rolling, "Abilities.Rolling", "Rolling Ability Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Knockback, "Abilities.Knockback", "Knockback Ability Tag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effects_HitReact, "Effects.HitReact", "HitReact");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_UI_Open, "Status.UI.Open", "MenuOpenTag");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputHeld, "Player.Block.InputHeld", "InputHeld Block");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputPressed, "Player.Block.InputPressed", "InputPressed Block");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Block_InputReleased, "Player.Block.InputReleased", "InputReleased Block");
}

