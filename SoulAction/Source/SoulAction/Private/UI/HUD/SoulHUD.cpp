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
	}
	return OverlayWidgetController;
}

void ASoulHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninit, Fill BP_HUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninit, Fill BP_HUD"));

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	
	OverlayWidget = CreateWidget<USoulUserWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		if (GetOverlayWidgetController(WidgetControllerParams))
		{
			OverlayWidget->SetWidgetController(OverlayWidgetController);
			OverlayWidget->AddToViewport();
		}
	}
}

void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();
}
