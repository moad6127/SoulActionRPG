// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/MenuWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

void UMenuWidgetController::BroadcastInitialValues()
{
}

void UMenuWidgetController::BindCallbacksToDependencies()
{
}

void UMenuWidgetController::InitSubWidgetController(const FWidgetControllerParams& WCParams)
{
	InitAttributeMenuWidgetController(WCParams);
}

void UMenuWidgetController::InitAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
}

UAttributeMenuWidgetController* UMenuWidgetController::GetAttributeWidgetController() const
{
	return AttributeMenuWidgetController;
}
