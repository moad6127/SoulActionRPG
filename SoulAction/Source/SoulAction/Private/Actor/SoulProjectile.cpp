// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SoulProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "SoulAction/SoulAction.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemLibrary.h"


ASoulProjectile::ASoulProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;


}

void ASoulProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASoulProjectile::OnSphereOverlap);

	LoopingSoundComponent =  UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	SetReplicateMovement(true);
}

void ASoulProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}

	Super::Destroyed();
}

void ASoulProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
	bHit = true;
}

void ASoulProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
	{
		return;
	}
	if (!USoulAbilitySystemLibrary::IsNotFriend(GetOwner(), OtherActor))
	{
		return;
	}
	if (!bHit)
	{
		OnHit();
	}


	if (HasAuthority())
	{
		//check(DamageEffectParans.SourceAbilitySystemComp);
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParans.DeathImpulseMagnitude;
			DamageEffectParans.DeathImpulse = DeathImpulse;
			
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParans.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParans.KnockbackForceMagnitude;
				DamageEffectParans.KnockbackForce = KnockbackForce;
			}
			DamageEffectParans.TargetAbilitySystemComp = TargetASC;
			USoulAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParans);
		}

		Destroy();
	}
	else
	{
		bHit = true;
	}
}


