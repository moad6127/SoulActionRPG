// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "SoulAction/SoulAction.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::Die()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Unequip(this);
	}
	MulticastHandleDeath();

}

void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	OnDied.Broadcast();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayeffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayeffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayeffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());

}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::InitStartWeapon(const FGameplayTag& WeaponTag)
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (SoulASC)
	{
		SoulASC->EquiWeaponByTag(WeaponTag);
	}
}

void ABaseCharacter::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abiilties)
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}

	SoulASC->AddCharacterAbilities(Abiilties);
}

FVector ABaseCharacter::GetCombatSocketLocation()
{
	if (!EquippedWeapon)
	{
		return FVector();
	}
	return EquippedWeapon->GetTipSocketLocation();
}

void ABaseCharacter::Equip(ABaseWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void ABaseCharacter::EquipWeapon(ABaseWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Unequip(this);
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->Equip(this);
	}
}

void ABaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterialInst))
	{
		UMaterialInstanceDynamic* DynamicInst = UMaterialInstanceDynamic::Create(DissolveMaterialInst, this);
		GetMesh()->SetMaterial(0, DynamicInst);

		StartDissolveTimeline(DynamicInst);
	}
}

