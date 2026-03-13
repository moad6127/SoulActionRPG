// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulAbilityTypes.h"
#include "SoulProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
UCLASS()
class SOULACTION_API ASoulProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoulProjectile();

	UProjectileMovementComponent* GetProjectileComp() { return ProjectileMovement; }
	USceneComponent* GetHomingTargetComponent() { return HomingTargetComponent; }
	void SetHomingTargetComponent(USceneComponent* InTarget) { HomingTargetComponent = InTarget; }

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParans;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void OnHit();

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetComponent;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 3.f;
};
