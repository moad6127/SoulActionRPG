// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SoulAbilitySystemComponent.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbiltiyTag*/, const FGameplayTag& /*StatusTag*/,int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbiltiyTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);

UCLASS()
class SOULACTION_API USoulAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void InitEquipWeapon(const FGameplayTag& WeaponTag);
	void EquiWeaponByTag(const FGameplayTag& WeaponTag);
	void EquipWeaponBody(const FGameplayTag& WeaponTag);
	void EquipWeaponAbilities(ABaseWeapon* SpawnedWeapon);
	FName GetEquippedWeaponName() const;

	UFUNCTION(Server, Reliable)
	void ServerEqiupWeaponByTag(const FGameplayTag& WeaponTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);


	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSepcFromInputTag(const FGameplayTag& InputTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	void UpgradeAttributeUseXP(const FGameplayTag& AttributeTag, int32 InXP);

	void UpdateAbilityStatus(int32 Level);

	UFUNCTION(Server,Reliable)
	void ServerSpendSpellXPPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	UFUNCTION(Server, Reliable)
	void ServerWeaponAbilityEquip(FGameplayAbilitySpecHandle AbilitySpecHandle);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
	
	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);

	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot); 

protected:

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbiltiyStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,int32 AbilityLevel);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttributeUseXP(const FGameplayTag& AttributeTag, int32 InXP);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UDataTable* WeaponDataTable;

private:

	TArray<FGameplayAbilitySpecHandle> EquippedWeaopnAbilitySpecHanle;

};
