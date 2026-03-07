// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulBeamSpell.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "SoulAction/SoulAction.h"

void USoulBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	MouseHitLocation = HitResult.ImpactPoint;
	MouseHitActor = HitResult.GetActor();
	/*
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	*/
}

void USoulBeamSpell::StoreOwnerVariable()
{

	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void USoulBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerCharacter);
		FHitResult HitResult;
		ABaseWeapon* CharacterEquippedWeapon = ICombatInterface::Execute_GetEquippedWeapon(OwnerCharacter);
		const FVector SocketLocation = CharacterEquippedWeapon->GetTipSocketLocation();
		FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);
		GetWorld()->SweepSingleByChannel(
			HitResult,
			SocketLocation,
			BeamTargetLocation,
			FQuat::Identity,
			ECC_Target,
			Sphere
		);

		if (HitResult.bBlockingHit)
		{
			MouseHitLocation = HitResult.ImpactPoint;
			MouseHitActor = HitResult.GetActor();
		}
	}

}
