// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "GameplayTagContainer.h" 
#include "OverlayWidgetController.generated.h"


struct FOnAttributeChangeData;
class USoulUserWidget;
class UAbilityInfo;
class USoulAbilitySystemComponent;
struct FSoulAbilityInfo;

USTRUCT(BlueprintType)
struct FUIWidgtRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USoulUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVitalChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgtRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FSoulAbilityInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API UOverlayWidgetController : public USoulWidgetController
{
	GENERATED_BODY()
	

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnVitalChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnVitalChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnVitalChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnVitalChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnVitalChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangeDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;

	void OnInitalizeStartupAbilities(USoulAbilitySystemComponent* SoulAbilitySystemComp);

	void OnXPChanged(int32 NewXP) const;

	template<typename T>
	T* GetDataTableRowByType(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByType(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
