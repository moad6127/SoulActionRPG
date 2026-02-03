// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulGameplayAbility.h"

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
