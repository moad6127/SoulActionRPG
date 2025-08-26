// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

void ABaseCharacter::AddCharacterAbilities()
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}

	SoulASC->AddCharacterAbilities(StartupAbilities);
}

FVector ABaseCharacter::GetCombatSocketLocation()
{
	if (!EquippedWeapon)
	{
		return FVector();
	}
	return EquippedWeapon->GetSocketLocation();
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

