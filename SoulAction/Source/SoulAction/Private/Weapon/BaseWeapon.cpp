// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Character/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Interaction/CombatInterface.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(GetRootComponent());

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(GetRootComponent());

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

void ABaseWeapon::HitScan()
{
	TArray<FHitResult> OutHits;
	FVector Start = TraceStart->GetComponentLocation();
	FVector End = TraceEnd->GetComponentLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());


	bool bHit = GetWorld()->SweepMultiByObjectType(
		OutHits,
		Start,
		End,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllDynamicObjects,
		Sphere,
		QueryParams
	);
	DrawDebugCapsule(
		GetWorld(),
		(Start + End) * 0.5f,
		FVector::Distance(Start, End) * 0.5f,
		TraceRadius,
		FRotationMatrix::MakeFromZ(End - Start).ToQuat(),
		FColor::Purple,
		false,
		0.1f
	);

	FColor TraceColor = bHit ? FColor::Red : FColor::Green;

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		TraceColor,
		false,
		0.1f,
		0,
		2.0f
	);

	if (!bHit)
	{
		return;
	}
	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor->Implements<UCombatInterface>())	return;
		if (ICombatInterface::Execute_IsDead(HitActor)) return;
		if (TraceHitActors.Contains(HitActor))	continue;
		TraceHitActors.Add(HitActor);
		OnWeaponHit.Broadcast(HitActor);
	}
}

void ABaseWeapon::HitScanStart()
{
	if (!HasAuthority())
	{
		return;
	}
	TraceHitActors.Empty();

	GetWorld()->GetTimerManager().SetTimer(
	HitScanTimerHandle,
		this,
		&ABaseWeapon::HitScan,
		HitScanInterval,
		true
	);
}

void ABaseWeapon::HitScanEnd()
{
	if (!HasAuthority())
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(HitScanTimerHandle);
}

