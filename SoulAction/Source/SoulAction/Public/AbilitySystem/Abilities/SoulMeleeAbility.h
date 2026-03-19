// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulDamageGameplayAbility.h"
#include "SoulMeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class SOULACTION_API USoulMeleeAbility : public USoulDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

protected:


	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bIsComboWindow = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bReceivedInputAction = false;

private:
};
