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

	EquipWeaponBody(WeaponTag);
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

FName USoulAbilitySystemComponent::GetEquippedWeaponName() const
{
	if (GetAvatarActor()->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetEquippedWeaponName(GetAvatarActor());
	}
	return FName();
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

FGameplayTag USoulAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag USoulAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
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
			ClientUpdateAbiltiyStatus(Info.AbilityTag, SoulGameplayTags::Abilities_Status_Eligible, 1);
		}
	}
}

void USoulAbilitySystemComponent::ServerSpendSpellXPPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (!GetAvatarActor()->Implements<UPlayerInterface>())
		{
			return;
		}
		if (Status.MatchesTagExact(SoulGameplayTags::Abilities_Status_Eligible))
		{
			//어빌리티 언락
			if (IPlayerInterface::Execute_GetXP(GetAvatarActor()) < 1000)
			{
				return;
			}
			AbilitySpec->DynamicAbilityTags.RemoveTag(SoulGameplayTags::Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(SoulGameplayTags::Abilities_Status_Unlocked);
			Status = SoulGameplayTags::Abilities_Status_Unlocked;
			IPlayerInterface::Execute_AddToXP(GetAvatarActor(), -1000);
		}
		else if (Status.MatchesTagExact(SoulGameplayTags::Abilities_Status_Equipped) || Status.MatchesTagExact(SoulGameplayTags::Abilities_Status_Unlocked))
		{
			if (IPlayerInterface::Execute_GetXP(GetAvatarActor()) < AbilitySpec->Level * 100)
			{
				return;
			}
			//어빌리티 레벨업
			IPlayerInterface::Execute_AddToXP(GetAvatarActor(), -1 * (AbilitySpec->Level * 100));
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbiltiyStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}

}

void USoulAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == SoulGameplayTags::Abilities_Status_Equipped || Status == SoulGameplayTags::Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			//현재 InputTag를 사용하는 모든 Ability제거하기
			ClearAbilitiesOfSlot(Slot);
			// 현재 선택된 Ability의 Slot제거하기
			ClearSlot(AbilitySpec);
			// Slot을 Ability에 넣기
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);
			if (Status.MatchesTagExact(SoulGameplayTags::Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(SoulGameplayTags::Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(SoulGameplayTags::Abilities_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, SoulGameplayTags::Abilities_Status_Equipped, Slot, PrevSlot);
	}
}

void USoulAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PrevSlot);
}

bool USoulAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = SoulAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = SoulAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = USoulAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(SoulGameplayTags::Abilities_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = USoulGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void USoulAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void USoulAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveSlpcedLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool USoulAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
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

void USoulAbilitySystemComponent::ClientUpdateAbiltiyStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
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
