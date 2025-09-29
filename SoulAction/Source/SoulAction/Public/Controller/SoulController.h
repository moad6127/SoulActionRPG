// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "SoulController.generated.h"

/**
 * 
 */

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class USoulInputConfig;
class USoulAbilitySystemComponent;
class UDamageTextComponent;

UCLASS()
class SOULACTION_API ASoulController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	bool bTargetLock = false;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> SoulIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttributeMenu;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> TargetLockAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<USoulInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<USoulAbilitySystemComponent> SoulAbilitySystemComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamagetextComponentClass;

	USoulAbilitySystemComponent* GetASC();

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void ShowAttributeMenu();
	void SetTargetLock();

};
