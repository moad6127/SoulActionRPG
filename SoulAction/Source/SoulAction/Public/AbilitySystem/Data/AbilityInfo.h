// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FSoulAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> ICon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};
/**
 * 
 */
UCLASS()
class SOULACTION_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FSoulAbilityInfo> AbilityInformation;

	FSoulAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
