// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"

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
	const USoulAttributeSet* SAS = CastChecked<USoulAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetMaxStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);

	Cast<USoulAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString MSG = FString::Printf(TEXT("GE Tag : %s"), *Tag.GetTagName().ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, MSG);

				FUIWidgtRow* Row = GetDataTableRowByType<FUIWidgtRow>(MessageWidgetDataTable, Tag);
			}
		}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
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
