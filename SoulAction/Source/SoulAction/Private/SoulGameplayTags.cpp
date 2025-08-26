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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_ArmorPertration, "Attributes.Secondary.ArmorPertration", "ArmorPertration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance", "CriticalHitChance");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage", "Increase Criticla Hit Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration", "HealthRegeneration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_StaminaRegeneration, "Attributes.Secondary.StaminaRegeneration", "StaminaRegeneration");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth", "MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_MaxStamina, "Attributes.Secondary.MaxStamina", "MaxStamina");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "InputTag LMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "InputTag RMB");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Shift, "InputTag.Shift", "InputTag Shift");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponTag_Staff, "WeaponTag.Staff", "WeaponTag Staff");
}
