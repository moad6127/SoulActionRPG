// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SoulHUD.h"
#include "UI/Widget/SoulUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/SoulWidgetController.h"

#include "UI/WidgetController/MenuWidgetController.h"

UOverlayWidgetController* ASoulHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UMenuWidgetController* ASoulHUD::GetMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (MenuWidgetController == nullptr)
	{
		MenuWidgetController = NewObject<UMenuWidgetController>(this, MenuWidgetControllerClass);
		MenuWidgetController->SetWidgetControllerParams(WCParams);
		MenuWidgetController->BindCallbacksToDependencies();
	}
	return MenuWidgetController;
}

void ASoulHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninit, Fill BP_HUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninit, Fill BP_HUD"));
	checkf(MenuClass, TEXT("MenuClass uninit, Fill BP_HUD"));
	checkf(MenuWidgetControllerClass, TEXT("MenuWidgetControllerClass uninit, FILL BP_HUD"));

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	
	OverlayWidget = CreateWidget<USoulUserWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		if (GetOverlayWidgetController(WidgetControllerParams))
		{
			OverlayWidget->SetWidgetController(OverlayWidgetController);
			OverlayWidgetController->BroadcastInitialValues();

			OverlayWidget->AddToViewport();
		}
	}

	Menu = CreateWidget<USoulUserWidget>(GetWorld(),MenuClass);
	if (Menu)
	{
		if (GetMenuWidgetController(WidgetControllerParams))
		{
			MenuWidgetController->InitSubWidgetController(WidgetControllerParams);
			Menu->SetWidgetController(MenuWidgetController);

			Menu->AddToViewport();
			Menu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void ASoulHUD::ShowMenu()
{

}


void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulHUD::HideMenu()
{
	if (Menu)
	{
		Menu->SetVisibility(ESlateVisibility::Hidden);
		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
}

void ASoulHUD::DisplayMenu()
{
	if (Menu)
	{
		Menu->SetVisibility(ESlateVisibility::Visible);
		const FInputModeGameAndUI InputMode;

		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}
