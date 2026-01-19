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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgtRow, Row);

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


	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnVitalChangedSignature OnXPPercentChangedDelegate;



protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget|Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;



	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;

	void OnXPChanged(int32 NewXP);

	template<typename T>
	T* GetDataTableRowByType(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByType(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
