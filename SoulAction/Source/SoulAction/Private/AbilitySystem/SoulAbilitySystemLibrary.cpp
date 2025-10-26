// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SoulHUD.h"
#include "Controller/SoulPlayerState.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "Game/SoulGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "SoulAbilityTypes.h"
#include "Interaction/CombatInterface.h"

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

void USoulAbilitySystemLibrary::GiveStartupAbities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
		if (CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}

}

UCharacterClassInfo* USoulAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ASoulGameModeBase* SoulGameMode = Cast<ASoulGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SoulGameMode == nullptr)
	{
		return nullptr;
	}

	return SoulGameMode->CharacterClassInfo;
}

bool USoulAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->IsBlockedHit();
	}
	return false;
}

bool USoulAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->IsCriticalHit();
	}
	return false;
}

void USoulAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void USoulAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

