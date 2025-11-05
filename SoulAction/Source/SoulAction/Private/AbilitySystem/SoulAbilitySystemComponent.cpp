// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/Abilities/SoulGameplayAbility.h"
#include "Weapon/BaseWeapon.h"
#include "Interaction/CombatInterface.h"

void USoulAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USoulAbilitySystemComponent::ClientEffectApplied);
}

void USoulAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(SoulAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void USoulAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USoulAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void USoulAbilitySystemComponent::EquiWeaponByTag(const FGameplayTag& WeaponTag)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/AbilitySystem/Data/DT_WeaponDataTable"));
	if (!WeaponDataTable)
	{
		return;
	}
	FName RowName = WeaponTag.GetTagName();
	if (const FWeaponDataRow* Row = WeaponDataTable->FindRow<FWeaponDataRow>(RowName, FString("")))
	{
		if (ICombatInterface* Combat =Cast<ICombatInterface>(GetAvatarActor()))
		{
			if (!Combat->bInitWeaponGet())
			{
				return;
			}
			ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(Row->WeaponClass);
			Combat->Equip(SpawnedWeapon);
		}
	}
}

void USoulAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
