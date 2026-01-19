// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MenuWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/WeaponMenuWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

void UMenuWidgetController::BroadcastInitialValues()
{
}

void UMenuWidgetController::BindCallbacksToDependencies()
{
}

void UMenuWidgetController::InitSubWidgetController(const FWidgetControllerParams& WCParams)
{
	InitAttributeMenuWidgetController(WCParams);
	InitWeaponMenuWidgetController(WCParams);
}

void UMenuWidgetController::InitAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetControllerClass == nullptr)
	{
		return;
	}
	AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
	AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
	AttributeMenuWidgetController->BindCallbacksToDependencies();
}

void UMenuWidgetController::InitWeaponMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (WeaponMenuControllerClass == nullptr)
	{
		return;
	}
	WeaponMenuController = NewObject<UWeaponMenuWidgetController>(this, WeaponMenuControllerClass);
	WeaponMenuController->SetWidgetControllerParams(WCParams);
	WeaponMenuController->BindCallbacksToDependencies();
}

void UMenuWidgetController::InitSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetControllerClass == nullptr)
	{
		return;
	}
	SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
	SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
	SpellMenuWidgetController->BindCallbacksToDependencies();

}

UAttributeMenuWidgetController* UMenuWidgetController::GetAttributeWidgetController() const
{
	if (AttributeMenuWidgetController)
	{
		return AttributeMenuWidgetController;
	}
	return nullptr;
}

UWeaponMenuWidgetController* UMenuWidgetController::GetWeaponMenuWidgetController() const
{
	if (WeaponMenuController)
	{
		return WeaponMenuController;
	}
	return nullptr;
}
