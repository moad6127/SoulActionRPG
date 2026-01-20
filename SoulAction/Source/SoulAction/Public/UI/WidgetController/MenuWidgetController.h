// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "MenuWidgetController.generated.h"

/**
 * 
 */
class UAttributeMenuWidgetController;
class UWeaponMenuWidgetController;
class USpellMenuWidgetController;



UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API UMenuWidgetController : public USoulWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	void InitSubWidgetController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	UAttributeMenuWidgetController* GetAttributeWidgetController() const;

	UFUNCTION(BlueprintCallable)
	UWeaponMenuWidgetController* GetWeaponMenuWidgetController() const;

	UFUNCTION(BlueprintCallable)
	USpellMenuWidgetController* GetSpellMenuWidgetController() const;
private:
	void InitAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitWeaponMenuWidgetController(const FWidgetControllerParams& WCParams);
	void InitSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UWeaponMenuWidgetController> WeaponMenuController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeaponMenuWidgetController> WeaponMenuControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
