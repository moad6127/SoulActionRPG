// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
class USoulUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelctedSignature, USoulUserWidget*, AbilityButton);

UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API USpellMenuWidgetController : public USoulWidgetController
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FAbilitySelctedSignature AbilitySelecteDelegate;

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SelectAbility(USoulUserWidget* AbilityButton);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Menu")
	FOnPlayerStatChangedSignature XPPointsChangeDelegate;

};
