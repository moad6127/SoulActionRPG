// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoulHUD.generated.h"

/**
 * 
 */
class USoulUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class SOULACTION_API ASoulHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<USoulUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void ShowMenu();
protected:
	virtual void BeginPlay() override;

	void HideMenu();
	void DisplayMenu();

private:

	UPROPERTY()
	TObjectPtr<USoulUserWidget> Menu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USoulUserWidget> MenuClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;


	bool bVisibleMenu;
};
