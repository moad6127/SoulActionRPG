// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "GameplayTagContainer.h"
#include "SoulGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
class USoulUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelctedSignature, USoulUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignatrue, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelection, const FGameplayTag&, AbilityType);


struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

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
	void DeselectAbility();

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GetAllAbilityInfos(TArray<FSoulAbilityInfo>& OutInfos);

	UFUNCTION(BlueprintCallable)
	FName GetEquippedWeaponName();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Menu")
	FOnPlayerStatChangedSignature XPPointsChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAbilitySelctedSignature AbilitySelecteDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignatrue SpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelection WaitForEquipSelection;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelection StopWaitingForEquipDelegate;
private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 XPPoints, int32 AbilityLevel, bool& bShouldEnabledSpendPointButton, bool& bShouldEnableEquipButton);

	FSelectedAbility SelectedAbility = { SoulGameplayTags::Abilities_None, SoulGameplayTags::Abilities_Status_Locked };
	int32 CurrentXP = 0;

	bool bWaitingForEquipSelection = false;
};
