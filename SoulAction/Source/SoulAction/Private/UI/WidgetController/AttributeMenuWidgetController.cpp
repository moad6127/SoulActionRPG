// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "SoulGameplayTags.h"
#include "Controller/SoulPlayerState.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

	check(AttributeInfomation);

	for (auto& Pair : GetSoulAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	GetSoulPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points) 
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
	GetSoulPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 InXP) 
		{
			XPPointsChangeDelegate.Broadcast(InXP);
		}
	);
}



void UAttributeMenuWidgetController::BroadcastInitialValues()
{

	check(AttributeInfomation);

	for (auto& Pair : GetSoulAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	AttributePointsChangedDelegate.Broadcast(GetSoulPS()->GetAttributePoints());
	XPPointsChangeDelegate.Broadcast(GetSoulPS()->GetXP());
}


void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetSoulASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::UpgradeAttributeUseXP(const FGameplayTag& AttributeTag)
{
	FGameplayAttribute Attribute = GetSoulAS()->TagsToAttributes[AttributeTag]();
	float Value = Attribute.GetNumericValue(AttributeSet);

	int32 NeedXP = Value * 100;
	if (GetSoulPS()->GetXP() >= NeedXP)
	{
		GetSoulASC()->UpgradeAttributeUseXP(AttributeTag, NeedXP);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	FSoulAttributeInfo Info = AttributeInfomation->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


