// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/WeaponMenuWidgetController.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"

void UWeaponMenuWidgetController::BroadcastInitialValues()
{
}

void UWeaponMenuWidgetController::BindCallbacksToDependencies()
{
}

void UWeaponMenuWidgetController::EquipWeapon(const FGameplayTag& WeaponTag)
{
	USoulAbilitySystemComponent* ASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (ASC)
	{
		ASC->EquiWeaponByTag(WeaponTag);
	}
}
