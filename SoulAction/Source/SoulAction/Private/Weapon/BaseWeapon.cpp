// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"
#include "Net/UnrealNetwork.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetIsReplicated(true);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

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
	if (!HasAuthority())
	{
		return;
	}

	SetOwner(nullptr);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	bIsDropped = true;
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody);
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

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, bIsDropped);
}

void ABaseWeapon::OnRep_Dropped()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody);
		WeaponMesh->SetEnableGravity(true);
	}
}



