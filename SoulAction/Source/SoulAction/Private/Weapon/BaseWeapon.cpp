// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetSimulatePhysics(false);
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
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetEnableGravity(true);
	}
	SetLifeSpan(3.f);
	//UE_LOG(LogTemp, Warning, TEXT("UnEquipWeapon: %s"), *this->GetName());
}

FVector ABaseWeapon::GetTipSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(WeaponTipSocketName);
}

FName ABaseWeapon::GetAttachWeaponSocketName() const
{
	return AttachWeaponSocketName;
}



