// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulBeamSpell.h"
#include "GameFramework/Character.h"

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
