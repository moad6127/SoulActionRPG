// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulAbilitySystemLibrary.generated.h"

/**
 * 
 */
class UOverlayWidgetController;

UCLASS()
class SOULACTION_API USoulAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMeuWidgetController(const UObject* WorldContextObject);
};
