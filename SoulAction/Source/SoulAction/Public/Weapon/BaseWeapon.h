// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "BaseWeapon.generated.h"

class UGameplayAbility;
class ABaseCharacter;
class UBlendSpace;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHitSignature, AActor*, HitActor);

struct FWeaponAinmations
{
	TObjectPtr<UBlendSpace> WeaponRun;
};

UCLASS()
class SOULACTION_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();
	virtual void Equip(class ABaseCharacter* Character);
	virtual void Unequip(ABaseCharacter* Character);

	FGameplayTag GetWeaponType() const { return WeaponType; }
	const TArray<TSubclassOf<UGameplayAbility>>& GetGrantedAbilities() const { return GrantedAbilities; }
	FVector GetTipSocketLocation() const;
	FName GetAttachWeaponSocketName() const;
	FName GetWeaponName() const { return WeaponName; }
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	UBlendSpace* GetWeaponRunAnim() const { return WeaponRunAnim; }
	UBlendSpace* GetWeaponTargetAnim() const { return WeaponTargetOnAnim; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void HitScanStart();

	UFUNCTION(BlueprintCallable)
	void HitScanEnd();

	UPROPERTY(BlueprintAssignable)
	FOnWeaponHitSignature OnWeaponHit;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent>WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName AttachWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName WeaponName;

	// 이 무기가 제공하는 Ability (추후 GAS 연결)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<UBlendSpace> WeaponRunAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TObjectPtr<UBlendSpace> WeaponTargetOnAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USceneComponent> TraceStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USceneComponent> TraceEnd;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TraceRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float HitScanInterval = 0.07f;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TSet<AActor*> TraceHitActors;
private:

	UPROPERTY(ReplicatedUsing = OnRep_Dropped)
	bool bIsDropped = false;

	FTimerHandle HitScanTimerHandle;


	UFUNCTION()
	void OnRep_Dropped();

	void HitScan();
};
