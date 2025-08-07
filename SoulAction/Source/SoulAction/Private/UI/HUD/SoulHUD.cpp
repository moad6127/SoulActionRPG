// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SoulHUD.h"
#include "UI/Widget/SoulUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

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

void ASoulHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninit, Fill BP_HUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninit, Fill BP_HUD"));
	checkf(MenuClass, TEXT("AttributeMenuClass uninit, Fill BP_HUD"));

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
		Menu->SetWidgetController(OverlayWidgetController);
		Menu->AddToViewport();
		Menu->SetVisibility(ESlateVisibility::Hidden);
		bVisibleMenu = false;
	}
	
}

void ASoulHUD::ShowMenu()
{
	if (bVisibleMenu)
	{
		HideMenu();
	}
	else
	{
		DisplayMenu();
	}
}

void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulHUD::HideMenu()
{
	if (Menu)
	{
		bVisibleMenu = false;
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
		bVisibleMenu = true;
		Menu->SetVisibility(ESlateVisibility::Visible);
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}
