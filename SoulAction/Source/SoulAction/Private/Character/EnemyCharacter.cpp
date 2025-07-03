// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/SoulAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>("AbilitySystemCopmonent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<USoulAttributeSet>("AttributeSet");

}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
