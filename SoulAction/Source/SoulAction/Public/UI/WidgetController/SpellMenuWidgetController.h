// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
class USoulUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelctedSignature, USoulUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignatrue, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled);

UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API USpellMenuWidgetController : public USoulWidgetController
{
	GENERATED_BODY()
public:


	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SelectAbility(USoulUserWidget* AbilityButton);

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Menu")
	FOnPlayerStatChangedSignature XPPointsChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAbilitySelctedSignature AbilitySelecteDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignatrue SpellGlobeSelectedDelegate;

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 XPPoints, int32 AbilityLevel, bool& bShouldEnabledSpendPointButton, bool& bShouldEnableEquipButton);
};
