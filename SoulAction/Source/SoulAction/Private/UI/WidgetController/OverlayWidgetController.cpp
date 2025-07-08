// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const USoulAttributeSet* SAS = CastChecked<USoulAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(SAS->GetHealth());
	OnMaxHealthChanged.Broadcast(SAS->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const USoulAttributeSet* SAS = CastChecked<USoulAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
