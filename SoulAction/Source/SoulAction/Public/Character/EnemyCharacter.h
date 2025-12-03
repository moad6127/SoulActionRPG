// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
class UWidgetComponent;
class UBehaviorTree;
class ASoulAIController;

UCLASS()
class SOULACTION_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void PossessedBy(AController* NewController) override;

	/* CombatInterface*/
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/* CombatInterface*/

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget);
	virtual AActor* GetCombatTarget_Implementation() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UPROPERTY(BlueprintAssignable)
	FOnVitalChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnVitalChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defautls")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defautls")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReact = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category ="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ASoulAIController> SoulAIController;
};
