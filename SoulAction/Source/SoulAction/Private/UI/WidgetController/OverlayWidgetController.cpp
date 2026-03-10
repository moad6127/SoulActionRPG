// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Controller/SoulPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "SoulGameplayTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	
	OnHealthChanged.Broadcast(GetSoulAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetSoulAS()->GetMaxHealth());
	OnStaminaChanged.Broadcast(GetSoulAS()->GetStamina());
	OnMaxStaminaChanged.Broadcast(GetSoulAS()->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	GetSoulPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetSoulPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel) 
		{
			OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
		}
	);

	/*Labmda를 사용해서 Bind하기*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetSoulAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	/*
	* AddUObject를 사용해서 bind하기
	*/

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetSoulAS()->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetSoulAS()->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetSoulAS()->GetMaxStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);

	if (GetSoulASC())
	{
		GetSoulASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

		if (GetSoulASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetSoulASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetSoulASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgtRow* Row = GetDataTableRowByType<FUIWidgtRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}


void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}


void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	ULevelUpInfo* LevelupInfo = GetSoulPS()->LevelUpInfo;
	
	checkf(LevelupInfo, TEXT("LevelupInfo Error"));
	int32 Level = LevelupInfo->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelupInfo->LevelUpInfomation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelupInfo->LevelUpInfomation[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelupInfo->LevelUpInfomation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PrevLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / DeltaLevelRequirement;
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{

	// Slot에 이전에 장착된 Abiltiy 가 있을경우 장착된 Ability초기화 하기
	FSoulAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = SoulGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = SoulGameplayTags::Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
