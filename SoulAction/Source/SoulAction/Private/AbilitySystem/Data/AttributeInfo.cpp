// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FSoulAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FSoulAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] AttributeTag를 AttributeInfo에서 찾지 못했습니다."), *AttributeTag.ToString());
	}

	return FSoulAttributeInfo();
}
