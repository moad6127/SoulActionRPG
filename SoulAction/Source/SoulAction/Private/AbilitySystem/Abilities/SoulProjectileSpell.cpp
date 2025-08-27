// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulProjectileSpell.h"
#include "Actor/SoulProjectile.h"
#include "Interaction/CombatInterface.h"


void USoulProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer)
	{
		return;
	}
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO : Rotation 추가하기

		ASoulProjectile* Projectile =  GetWorld()->SpawnActorDeferred<ASoulProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO : 발사체에 데미지를 위해서 GameplayEffectSpec부여하기

		Projectile->FinishSpawning(SpawnTransform);
	}

}
