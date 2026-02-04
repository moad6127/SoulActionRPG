// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulGameplayAbility.h"
#include "AbilitySystem/SoulAttributeSet.h"

FString USoulGameplayAbility::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"),L"Default Ability Name - LoreIpsum LoreIpsum LoreIpsum LoreIpsum", Level);
}

FString USoulGameplayAbility::GetNextLevelDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>NextLevel: </><Level>%d</> \n <Default>Causes much more damage</>"), Level);
}

FString USoulGameplayAbility::GetLockedDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"),Level);
}

float USoulGameplayAbility::GetCost(float InLevel) const
{
    float CostValue = 0.f;
    if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == USoulAttributeSet::GetStaminaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, CostValue);
                break;
            }
        }
    }
    return CostValue;
}

float USoulGameplayAbility::GetCooldown(float InLevel) const
{
    float CooldownValue = 0.f;
    if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownValue);
    }
    return CooldownValue;
}
