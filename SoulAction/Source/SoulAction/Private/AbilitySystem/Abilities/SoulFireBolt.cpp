// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulFireBolt.h"
#include "SoulAction/Public/SoulGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "Actor/SoulProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
FString USoulFireBolt::GetDescription(int32 Level)
{

	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
			ScaledDamage
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
			ScaledDamage);
	}
}

FString USoulFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
		ScaledDamage);
}

void USoulFireBolt::SpawnProjectiles(AActor* HomingTarget, const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}
	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
			GetAvatarActorFromActorInfo(),
			SocketTag);
		//const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}

		const FVector Forward = Rotation.Vector();
		const int32 NumFireBolt = FMath::Min(NumProjectiles, GetAbilityLevel());

		TArray<FRotator> Rotations = USoulAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumFireBolt);

		for (const FRotator& Rot : Rotations)
		{
			FTransform SpawnTransform;

			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rot.Quaternion());


			ASoulProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASoulProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetAvatarActorFromActorInfo(),
				Cast<APawn>(GetAvatarActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			Projectile->DamageEffectParans = MakeDamageEffectPrarmsFromClassDefaults();

			Projectile->SetHomingTargetComponent(NewObject<USceneComponent>(USceneComponent::StaticClass()));
			Projectile->GetHomingTargetComponent()->SetWorldLocation(ProjectileTargetLocation);
			Projectile->GetProjectileComp()->HomingTargetComponent = Projectile->GetHomingTargetComponent();

			Projectile->GetProjectileComp()->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->GetProjectileComp()->bIsHomingProjectile = bLaunchHomingProjectiles;



			Projectile->FinishSpawning(SpawnTransform);
		}
	}

}
