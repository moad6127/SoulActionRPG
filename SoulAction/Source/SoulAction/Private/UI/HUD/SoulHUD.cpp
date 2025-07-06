// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SoulHUD.h"
#include "UI/Widget/SoulUserWidget.h"

void ASoulHUD::BeginPlay()
{
	Super::BeginPlay();

	if (OverlayWidgetClass)
	{
		OverlayWidget = CreateWidget<USoulUserWidget>(GetWorld(), OverlayWidgetClass);
		OverlayWidget->AddToViewport();
	}
}
