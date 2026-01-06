// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "SoulPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*SateValue*/);


UCLASS()
class SOULACTION_API ASoulPlayerState : public APlayerState ,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ASoulPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);

	FORCEINLINE int32 GetXP() const { return XP; }
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	void AddToAttributePoints(int32 InPoints);

	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	void AddToSpellPoints(int32 InPoints);

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
