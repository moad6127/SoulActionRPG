// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulFireBolt.h"
#include "SoulAction/Public/SoulGameplayTags.h"

FString USoulFireBolt::GetDescription(int32 Level)
{

	const int32 Damage = GetDamageByDamageType(Level, SoulGameplayTags::Damage_Fire);
	const float CostValue = FMath::Abs(GetCost(Level));
	const float CooldownValue = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Level
			"<Small>Level : </><Level>%d</>\n"
			//Cost
			"<Small>Cost : </><Cost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Launched a bolt of fire,"
			" exploding on impact and dealing: </>"
			"<Damage>%d</>"
			"<Default>fire Damage with a chance to burn</>\n\n"),

			//Value
			Level,
			CostValue,
			CooldownValue,
			Damage
			);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			//Cost
			"<Small>Cost : </><Cost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"

			"<Default>Launches %d bolt of fire,"
			" exploding on impact and dealing: </>"
			"<Damage>%d</>"
			"<Default>fire Damage with a chance to burn</>\n\n"),

			//Value
			Level,
			CostValue,
			CooldownValue,
			FMath::Min(Level,NumProjectiles),
			Damage);
	}
}

FString USoulFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, SoulGameplayTags::Damage_Fire);
	const float CostValue = FMath::Abs(GetCost(Level));
	const float CooldownValue = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>NEXT LEVEL</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//Cost
		"<Small>Cost : </><Cost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown : </><Cooldown>%.1f</>\n\n"

		"<Default>Launches %d bolt of fire,"
		" exploding on impact and dealing: </>"
		"<Damage>%d</>"
		"<Default>fire Damage with a chance to burn</>\n\n"),

		//Value
		Level,
		CostValue,
		CooldownValue,
		FMath::Min(Level, NumProjectiles),
		Damage);
}
