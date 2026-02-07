// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Controller/SoulPlayerState.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/SoulAttributeSet.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	if (GetSoulASC()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetSoulASC()->AbilitiesGivenDelegate.AddUObject(this, &USpellMenuWidgetController::BroadcastAbilityInfo);
	}

	SpellPointChanged.Broadcast(GetSoulPS()->GetSpellPoints());
	XPPointsChangeDelegate.Broadcast(GetSoulPS()->GetXP());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetSoulASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,int32 NewLevel) 
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				int32 SelectedAbilityLevel = 0;
				if (FGameplayAbilitySpec* Spec = GetSoulASC()->GetSpecFromAbilityTag(AbilityTag))
				{
					SelectedAbilityLevel = Spec->Level;
				}

				bool bEnableSpendPoint = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentXP, SelectedAbilityLevel, bEnableSpendPoint, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetSoulASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
			}
			if (AbilityInfo)
			{
				FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	// SpellPoint를 사용할경우 Point를 델리게이트로 보내기
	/*
	GetSoulPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
		{
			SpellPointChanged.Broadcast(SpellPoints);
		});
		*/

	GetSoulPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 InXP)
		{
			XPPointsChangeDelegate.Broadcast(InXP);
			CurrentXP = InXP;

			int32 SelectedAbilityLevel = 0;
			if (FGameplayAbilitySpec* Spec = GetSoulASC()->GetSpecFromAbilityTag(SelectedAbility.Ability))
			{
				SelectedAbilityLevel = Spec->Level;
			}

			bool bEnableSpendPoint = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentXP, SelectedAbilityLevel, bEnableSpendPoint, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetSoulASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SelectAbility(USoulUserWidget* AbilityButton)
{
	AbilitySelecteDelegate.Broadcast(AbilityButton);
}

void USpellMenuWidgetController::DeselectAbility()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = SoulGameplayTags::Abilities_None;
	SelectedAbility.Status = SoulGameplayTags::Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	// XPPoint
	const int32 XPPoints = GetSoulPS()->GetXP();
	FGameplayTag AbilityStatus;
	int32 AbilityLevel = 0;
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(SoulGameplayTags::Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetSoulASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (bSpecValid)
	{
		AbilityLevel = AbilitySpec->Level;
	}
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = SoulGameplayTags::Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetSoulASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;


	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, XPPoints, AbilityLevel, bEnableSpendPoint, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetSoulASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetSoulASC())
	{
		GetSoulASC()->ServerSpendSpellXPPoint(SelectedAbility.Ability);

	}
}

void USpellMenuWidgetController::GetAllAbilityInfos(TArray<FSoulAbilityInfo>& OutInfos)
{
	OutInfos.Empty();
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, &OutInfos](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = USoulAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
			if (!AbilityTag.IsValid())
			{
				return;
			}

			FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.InputTag = USoulAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = USoulAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
			OutInfos.Add(Info);
		});

	GetSoulASC()->ForEachAbility(BroadcastDelegate);
}

FName USpellMenuWidgetController::GetEquippedWeaponName()
{
	if (GetSoulASC())
	{
		return GetSoulASC()->GetEquippedWeaponName();
	}
	return FName();
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	WaitForEquipSelection.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 XPPoints, int32 AbilityLevel, bool& bShouldEnabledSpendPointButton, bool& bShouldEnableEquipButton)
{
	bShouldEnabledSpendPointButton = false;
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(SoulGameplayTags::Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (XPPoints >= AbilityLevel * 100)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(SoulGameplayTags::Abilities_Status_Eligible))
	{
		if (XPPoints >= AbilityLevel * 100)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(SoulGameplayTags::Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (XPPoints >= 1000)
		{
			bShouldEnabledSpendPointButton = true;
		}
	}
}
