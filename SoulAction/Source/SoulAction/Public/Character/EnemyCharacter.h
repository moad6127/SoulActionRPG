// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SOULACTION_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemyCharacter();


	/* CombatInterface*/
	virtual int32 GetPlayerLevel() override;
	/* CombatInterface*/
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defautls")
	int32 EnenyLevel = 1;


};
