// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class ABaseWeapon;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDied);

UCLASS(Abstract)
class SOULACTION_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual bool bInitWeaponGet() const override;

	virtual void StartDash_Implementation() override;
	virtual void EndDash_Implementation() override;

	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;

	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDied OnDied;

	UPROPERTY(EditAnywhere, Category ="Combat")
	TArray<FTaggedMontage> AttackMontage;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayeffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void InitStartWeapon(const FGameplayTag& WeaponTag);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abiilties);

	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual void Equip(class ABaseWeapon* Weapon) override;

	void EquipWeapon(ABaseWeapon* WeaponToEquip);

	void Dissolve();
	
	UFUNCTION()
	void OnRep_EquippedWeapon();

	void AttachEquippedWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicIns);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInst;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	bool bDead = false;
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bInitWeapon = true;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
