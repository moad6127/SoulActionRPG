// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Controller/SoulPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const USoulAttributeSet* SAS = CastChecked<USoulAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(SAS->GetHealth());
	OnMaxHealthChanged.Broadcast(SAS->GetMaxHealth());
	OnStaminaChanged.Broadcast(SAS->GetStamina());
	OnMaxStaminaChanged.Broadcast(SAS->GetMaxStamina());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	ASoulPlayerState* SoulPlayerState = CastChecked<ASoulPlayerState>(PlayerState);
	SoulPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	const USoulAttributeSet* SAS = CastChecked<USoulAttributeSet>(AttributeSet);
	/*Labmda를 사용해서 Bind하기*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	/*
	* AddUObject를 사용해서 bind하기
	*/

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetMaxStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);

	if (USoulAbilitySystemComponent* SoulASC = Cast<USoulAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (SoulASC->bStartupAbilitiesGiven)
		{
			OnInitalizeStartupAbilities(SoulASC);
		}
		else
		{
			SoulASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitalizeStartupAbilities);
		}

		SoulASC->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitalizeStartupAbilities(USoulAbilitySystemComponent* SoulAbilitySystemComp)
{
	if (!SoulAbilitySystemComp->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this] (const FGameplayAbilitySpec& AbilitySpec)
		{
			FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(USoulAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = USoulAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});

	SoulAbilitySystemComp->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const ASoulPlayerState* SoulPlayerState = CastChecked<ASoulPlayerState>(PlayerState);
	const ULevelUpInfo* LevelupInfo = SoulPlayerState->LevelUpInfo;

	checkf(LevelupInfo, TEXT("LevelupInfo Error"));
	int32 Level = LevelupInfo->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelupInfo->LevelUpInfomation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelupInfo->LevelUpInfomation[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelupInfo->LevelUpInfomation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PrevLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel / DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
