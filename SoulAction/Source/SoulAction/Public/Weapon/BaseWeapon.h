// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "BaseWeapon.generated.h"

class UGameplayAbility;
class ABaseCharacter;
class UBlendSpace;

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	TObjectPtr<UBlendSpace> WeaponRun;

private:

	UPROPERTY(ReplicatedUsing = OnRep_Dropped)
	bool bIsDropped = false;



	UFUNCTION()
	void OnRep_Dropped();
};
