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
}

void UAttributeMenuWidgetController::EquipWeapon(const FGameplayTag& WeaponTag)
{
	USoulAbilitySystemComponent* ASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (ASC)
	{
		ASC->EquiWeaponByTag(WeaponTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute)
{
	FSoulAttributeInfo Info = AttributeInfomation->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}


