// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "SoulGameplayTags.h"
#include "Controller/SoulPlayerState.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	USoulAttributeSet* AS = CastChecked<USoulAttributeSet>(AttributeSet);

	check(AttributeInfomation);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	ASoulPlayerState* SoulPlayerState = CastChecked<ASoulPlayerState>(PlayerState);
	SoulPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points) 
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
	SoulPlayerState->OnXPChangedDelegate.AddLambda(
		[this](int32 InXP) 
		{
			XPPointsChangeDelegate.Broadcast(InXP);
		}
	);
}



void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	USoulAttributeSet* AS = CastChecked<USoulAttributeSet>(AttributeSet);

	check(AttributeInfomation);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	ASoulPlayerState* SoulPlayerState = CastChecked<ASoulPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(SoulPlayerState->GetAttributePoints());
	XPPointsChangeDelegate.Broadcast(SoulPlayerState->GetXP());
}

void UAttributeMenuWidgetController::EquipWeapon(const FGameplayTag& WeaponTag)
{
	USoulAbilitySystemComponent* ASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (ASC)
	{
		ASC->EquiWeaponByTag(WeaponTag);
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	USoulAbilitySystemComponent* ASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	ASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::UpgradeAttributeUseXP(const FGameplayTag& AttributeTag)
{
	USoulAttributeSet* AS = CastChecked<USoulAttributeSet>(AttributeSet);
	FGameplayAttribute Attribute = AS->TagsToAttributes[AttributeTag]();
	float Value = Attribute.GetNumericValue(AttributeSet);

	ASoulPlayerState* SoulPlayerState = CastChecked<ASoulPlayerState>(PlayerState);
	int32 NeedXP = Value * 100;
	if (SoulPlayerState->GetXP() > NeedXP)
	{
		USoulAbilitySystemComponent* ASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
		ASC->UpgradeAttributeUseXP(AttributeTag, NeedXP);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	FSoulAttributeInfo Info = AttributeInfomation->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


