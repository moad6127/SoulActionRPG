// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SoulCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SOULACTION_API ASoulCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASoulCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* CombatInterface*/
	virtual int32 GetPlayerLevel() override;
	/* CombatInterface*/

	/*타겟 락온 관련 함수들*/
	void ToggleTargetLock();
	void FindLockOnTarget();
	void UpdateLockOnCamera(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	bool Targeting() const { return bTargetLockOn; };
	UFUNCTION()
	void OnTargetDied();

	ABaseCharacter* GetTargetActor() { return TargetActor; }

protected:
	virtual void BeginPlay() override;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float LockOnMaxRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float LockOnSphereRadius = 200.f;

	UPROPERTY(Replicated)
	bool bTargetLockOn = false;

	UPROPERTY()
	TObjectPtr<ABaseCharacter> TargetActor;
};
