// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "SoulGameplayTags.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	USoulAttributeSet* AS = CastChecked<USoulAttributeSet>(AttributeSet);

	check(AttributeInfomation);

	for (auto& Pair : AS->TagsToAttributes)
	{
		FSoulAttributeInfo Info = AttributeInfomation->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

