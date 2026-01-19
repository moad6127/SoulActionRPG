// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SoulWidgetController.h"
#include "Controller/SoulController.h"
#include "Controller/SoulPlayerState.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void USoulWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void USoulWidgetController::BroadcastInitialValues()
{
}

void USoulWidgetController::BindCallbacksToDependencies()
{
}

void USoulWidgetController::BroadcastAbilityInfo()
{
	if (!GetSoulASC()->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = USoulAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
			if (!AbilityTag.IsValid())
			{
				return;
			}

			FSoulAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.InputTag = USoulAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});

	GetSoulASC()->ForEachAbility(BroadcastDelegate);
}

ASoulController* USoulWidgetController::GetSoulPC()
{
	if (SoulPlayerController == nullptr)
	{
		SoulPlayerController = Cast<ASoulController>(PlayerController);
	}
	return SoulPlayerController;
}

ASoulPlayerState* USoulWidgetController::GetSoulPS()
{
	if (SoulPlayerState == nullptr)
	{
		SoulPlayerState = Cast<ASoulPlayerState>(PlayerState);
	}
	return SoulPlayerState;
}

USoulAbilitySystemComponent* USoulWidgetController::GetSoulASC()
{
	if (SoulAbilitySystemComponent == nullptr)
	{
		SoulAbilitySystemComponent = Cast<USoulAbilitySystemComponent>(AbilitySystemComponent);
	}
	return SoulAbilitySystemComponent;
}

USoulAttributeSet* USoulWidgetController::GetSoulAS()
{
	if (SoulAttributeSet == nullptr)
	{
		SoulAttributeSet = Cast<USoulAttributeSet>(AttributeSet);
	}
	return SoulAttributeSet;
}

