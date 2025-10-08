// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "SoulAbilitySystemLibrary.generated.h"

/**
 * 
 */
class UOverlayWidgetController;
class UAbilitySystemComponent;
UCLASS()
class SOULACTION_API USoulAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMeuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "SoulAbilitySystemLibrary|CharacterClassInfo")
	static void InitializeDefautlAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "SoulAbilitySystemLibrary|CharacterClassInfo")
	static void GiveStartupAbities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "SoulAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "SoulAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "SoulAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category = "SoulAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

};
