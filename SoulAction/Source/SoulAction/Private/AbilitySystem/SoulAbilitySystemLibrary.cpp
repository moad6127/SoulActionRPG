// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SoulHUD.h"
#include "Controller/SoulPlayerState.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "Game/SoulGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"

UOverlayWidgetController* USoulAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(PC->GetHUD()))
		{
			ASoulPlayerState* PS = PC->GetPlayerState<ASoulPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SoulHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* USoulAbilitySystemLibrary::GetAttributeMeuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(PC->GetHUD()))
		{
			ASoulPlayerState* PS = PC->GetPlayerState<ASoulPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SoulHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void USoulAbilitySystemLibrary::InitializeDefautlAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ASoulGameModeBase* SoulGameMode = Cast<ASoulGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SoulGameMode == nullptr)
	{
		return;
	}

	AActor* AvatarActor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo ClassDefautlInfo = SoulGameMode->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimayAttrbuteContextHandle = ASC->MakeEffectContext();
	PrimayAttrbuteContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefautlInfo.PrimaryAttributes, Level, PrimayAttrbuteContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttrbuteContextHandle = ASC->MakeEffectContext();
	SecondaryAttrbuteContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(SoulGameMode->CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttrbuteContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttrbuteContextHandle = ASC->MakeEffectContext();
	VitalAttrbuteContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalyAttributeSpecHandle = ASC->MakeOutgoingSpec(SoulGameMode->CharacterClassInfo->VitalAttributes, Level, VitalAttrbuteContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalyAttributeSpecHandle.Data.Get());

	Cast<USoulAbilitySystemComponent>(ASC)->EquiWeaponByTag(ClassDefautlInfo.WeaponTag);
}

void USoulAbilitySystemLibrary::GiveStartupAbities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	ASoulGameModeBase* SoulGameMode = Cast<ASoulGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SoulGameMode == nullptr)
	{
		return;
	}

	UCharacterClassInfo* CharacterClassInfo = SoulGameMode->CharacterClassInfo;
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}
