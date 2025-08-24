// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class SOULACTION_API ASoulProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoulProjectile();

	UProjectileMovementComponent* GetProjectileComp() { return ProjectileMovement; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
