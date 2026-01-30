// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Controller/SoulPlayerState.h"
#include "SoulGameplayTags.h"

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
	GetSoulASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) 
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				bool bEnableSpendPoint = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(StatusTag, CurrentXP, SelectedAbility.SelectedAbilityLevel, bEnableSpendPoint, bEnableEquip);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip);
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

			bool bEnableSpendPoint = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentXP, SelectedAbility.SelectedAbilityLevel, bEnableSpendPoint, bEnableEquip);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip);
		}
	);
}

void USpellMenuWidgetController::SelectAbility(USoulUserWidget* AbilityButton)
{
	AbilitySelecteDelegate.Broadcast(AbilityButton);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// XPPoint
	const int32 XPPoints = GetSoulPS()->GetXP();
	int32 AbilityLevel = 0;
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(SoulGameplayTags::Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetSoulASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (bSpecValid)
	{
		AbilityLevel = AbilitySpec->Ability->GetAbilityLevel();
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
	SelectedAbility.SelectedAbilityLevel = AbilityLevel;

	bool bEnableSpendPoint = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, XPPoints, AbilityLevel, bEnableSpendPoint, bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip);
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
