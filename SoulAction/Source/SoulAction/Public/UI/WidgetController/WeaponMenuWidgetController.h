// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "WeaponMenuWidgetController.generated.h"

/**
 * 
 */
struct FGameplayTag;

UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API UWeaponMenuWidgetController : public USoulWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;


	UFUNCTION(BlueprintCallable)
	void EquipWeapon(const FGameplayTag& WeaponTag);
	
};
