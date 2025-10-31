// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void ABaseWeapon::Equip(ABaseCharacter* Character)
{	
	if (Character)
	{
		SetOwner(Character);
	}
}

void ABaseWeapon::Unequip(ABaseCharacter* Character)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetLifeSpan(3.f);
}

FVector ABaseWeapon::GetTipSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

FName ABaseWeapon::GetAttachWeaponSocketName() const
{
	return AttachWeaponSocketName;
}



