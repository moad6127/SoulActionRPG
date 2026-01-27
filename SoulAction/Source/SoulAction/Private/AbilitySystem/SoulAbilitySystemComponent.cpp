// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/Abilities/SoulGameplayAbility.h"
#include "Weapon/BaseWeapon.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(SoulGameplayTags::Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void USoulAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
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

void USoulAbilitySystemComponent::InitEquipWeapon(const FGameplayTag& WeaponTag)
{
	if (!GetAvatarActor()->Implements<UCombatInterface>())
	{
		return;
	}
	if (!ICombatInterface::Execute_bInitWeaponGet(GetAvatarActor()))
	{
		return;
	}

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
		ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(Row->WeaponClass);
		ICombatInterface::Execute_Equip(GetAvatarActor(), SpawnedWeapon);
	}
}

void USoulAbilitySystemComponent::EquiWeaponByTag(const FGameplayTag& WeaponTag)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerEqiupWeaponByTag(WeaponTag);
	}
	else
	{
		EquipWeaponBody(WeaponTag);
	}
}

void USoulAbilitySystemComponent::ServerEqiupWeaponByTag_Implementation(const FGameplayTag& WeaponTag)
{
	EquipWeaponBody(WeaponTag);
}

void USoulAbilitySystemComponent::EquipWeaponBody(const FGameplayTag& WeaponTag)
{
	if (!GetAvatarActor()->Implements<UCombatInterface>())
	{
		return;
	}

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
		ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(Row->WeaponClass);
		ICombatInterface::Execute_Equip(GetAvatarActor(), SpawnedWeapon);
	}
}

void USoulAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag USoulAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag USoulAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag USoulAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* USoulAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActivaeScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

void USoulAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void USoulAbilitySystemComponent::UpgradeAttributeUseXP(const FGameplayTag& AttributeTag, int32 InXP)
{
	ServerUpgradeAttributeUseXP(AttributeTag,InXP);
}

void USoulAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = USoulAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FSoulAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		if (Level < Info.LevelRequirement)
		{
			continue;
		}
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(SoulGameplayTags::Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbiltiyStatus(Info.AbilityTag, SoulGameplayTags::Abilities_Status_Eligible);
		}
	}
}

void USoulAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void USoulAbilitySystemComponent::ClientUpdateAbiltiyStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}



void USoulAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void USoulAbilitySystemComponent::ServerUpgradeAttributeUseXP_Implementation(const FGameplayTag& AttributeTag, int32 InXP)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToXP(GetAvatarActor(), -1 * InXP);
		IPlayerInterface::Execute_LevelUp(GetAvatarActor());
		IPlayerInterface::Execute_AddToPlayerLevel(GetAvatarActor(), 1);
	}
}

void USoulAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
