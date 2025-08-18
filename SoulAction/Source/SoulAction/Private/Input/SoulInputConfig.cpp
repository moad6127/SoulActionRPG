// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/SoulInputConfig.h"

const UInputAction* USoulInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FSoulInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilityINputAction을 찾지 못했습니다."));
	}

	return nullptr;
}
