// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAttributeSet.h"

#include "Net/UnrealNetwork.h"

USoulAttributeSet::USoulAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitStamina(200.f);
	InitMaxStamina(200.f);
}

void USoulAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USoulAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void USoulAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void USoulAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const 
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Health, OldHealth);
}

void USoulAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxHealth, OldMaxHealth);

}

void USoulAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, Stamina, OldStamina);
}

void USoulAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USoulAttributeSet, MaxStamina, OldMaxStamina);
}
