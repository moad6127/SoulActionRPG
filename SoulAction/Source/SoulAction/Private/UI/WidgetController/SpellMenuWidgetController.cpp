// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"

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
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{

}
