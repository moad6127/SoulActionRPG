// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataSignature, const FVector&, Data);

UCLASS()
class SOULACTION_API UTargetData : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Task", meta = (DisplayName = "TargetData" ,HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetData* CreateTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FTargetDataSignature ValidData;

private:
	virtual void Activate() override;
};
