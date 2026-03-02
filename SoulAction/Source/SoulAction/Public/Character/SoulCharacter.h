// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "SoulCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

UCLASS()
class SOULACTION_API ASoulCharacter : public ABaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ASoulCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* PlayerInterface*/
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoins) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoins) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	/* PlayerInterface*/


	/* CombatInterface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	/* CombatInterface*/

	/*타겟 락온 관련 함수들*/
	void ToggleTargetLock();

	UFUNCTION(Server, Reliable)
	void ServerToggleTargetLock(ABaseCharacter* RequestedTarget);

	void FindLockOnTarget();
	void UpdateLockOnCamera(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	bool Targeting() const { return bTargetLockOn; };
	UFUNCTION()
	void OnTargetDied();

	ABaseCharacter* GetTargetActor() { return TargetActor; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly )
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComp;

	UPROPERTY(BlueprintReadWrite)
	bool bShockLoop = false;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION()
	void OnRep_bTargeting();

	void TargetLockOnMovementSetting();

	UFUNCTION()
	void OnRep_TargetActor();

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float LockOnMaxRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float LockOnSphereRadius = 200.f;

	UPROPERTY(ReplicatedUsing = OnRep_bTargeting)
	bool bTargetLockOn = false;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float CameraInterpSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float CharacterInterpSpeed = 10.f;

	UPROPERTY(ReplicatedUsing = OnRep_TargetActor)
	TObjectPtr<ABaseCharacter> TargetActor;
};
