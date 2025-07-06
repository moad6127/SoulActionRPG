// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulHUD.generated.h"

/**
 * 
 */
class USoulUserWidget;

UCLASS()
class SOULACTION_API ASoulHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<USoulUserWidget> OverlayWidget;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulUserWidget> OverlayWidgetClass;
};
