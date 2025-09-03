// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

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

	/*
	* Projectile을 발사하는 능력을 사용할때 발사체가 발사될 방향을 정하기 위해서 LineTrace를 사용해서
	* 화면의 중앙을 선택한다.
	*/
	void GetTraceHitResult(FHitResult& TraceHitResult);

	virtual void Activate() override;
	void SendTargetData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
