// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void ABaseWeapon::Equip(ABaseCharacter* Character)
{
	if (Character && WeaponMesh)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		WeaponMesh->AttachToComponent(Character->GetMesh(), AttachRules, AttachWeaponSocketName);
	}
}

void ABaseWeapon::Unequip(ABaseCharacter* Character)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

FVector ABaseWeapon::GetSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}



