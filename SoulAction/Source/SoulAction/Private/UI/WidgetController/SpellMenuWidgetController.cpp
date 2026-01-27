// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Controller/SoulPlayerState.h"

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
			if (AbilityInfo)
			{
				FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});
	GetSoulPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
		{
			SpellPointChanged.Broadcast(SpellPoints);
		});
	GetSoulPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 InXP)
		{
			XPPointsChangeDelegate.Broadcast(InXP);
		}
	);
}
