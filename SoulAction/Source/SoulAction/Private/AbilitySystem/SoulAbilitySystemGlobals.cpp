// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemGlobals.h"
#include "SoulAbilityTypes.h"

FGameplayEffectContext* USoulAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSoulGameplayEffectContext(); 
}
