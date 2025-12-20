// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SoulPlayerState.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "Net/UnrealNetwork.h"

ASoulPlayerState::ASoulPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>("AbilitySystemCopmonent");

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full); // 멀티도 가능하지만 싱글로 만들거라서 Full로 설정

	AttributeSet = CreateDefaultSubobject<USoulAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ASoulPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulPlayerState, Level);
	DOREPLIFETIME(ASoulPlayerState, XP);
}

UAbilitySystemComponent* ASoulPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASoulPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ASoulPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ASoulPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ASoulPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ASoulPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ASoulPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
