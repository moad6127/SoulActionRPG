// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SoulDamageGameplayAbility.h"
#include "SoulProjectileSpell.generated.h"

/**
 * 
 */
class ASoulProjectile;
class UGameplayEffect;
struct FGameplayTag;

UCLASS()
class SOULACTION_API USoulProjectileSpell : public USoulDamageGameplayAbility
{
	GENERATED_BODY()
	

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASoulProjectile> ProjectileClass;



};
