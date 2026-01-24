// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SoulHUD.h"
#include "Controller/SoulPlayerState.h"
#include "UI/WidgetController/MenuWidgetController.h"
#include "Game/SoulGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "SoulAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "Engine/OverlapResult.h"

bool USoulAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ASoulHUD*& OutSoulHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutSoulHUD = Cast<ASoulHUD>(PC->GetHUD());
		if (OutSoulHUD)
		{
			ASoulPlayerState* PS = PC->GetPlayerState<ASoulPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* USoulAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASoulHUD* SoulHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SoulHUD))
	{
		return SoulHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* USoulAbilitySystemLibrary::GetAttributeMeuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASoulHUD* SoulHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SoulHUD))
	{
		return SoulHUD->GetMenuWidgetController(WCParams)->GetAttributeWidgetController();
	}
	return nullptr;
}

USpellMenuWidgetController* USoulAbilitySystemLibrary::GetSpellMeuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASoulHUD* SoulHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SoulHUD))
	{
		return SoulHUD->GetMenuWidgetController(WCParams)->GetSpellMenuWidgetController();
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

		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}

}

UCharacterClassInfo* USoulAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ASoulGameModeBase* SoulGameMode = Cast<ASoulGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SoulGameMode == nullptr)
	{
		return nullptr;
	}

	return SoulGameMode->CharacterClassInfo;
}

UAbilityInfo* USoulAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ASoulGameModeBase* SoulGameMode = Cast<ASoulGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SoulGameMode == nullptr)
	{
		return nullptr;
	}

	return SoulGameMode->AbilityInfo;
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

void USoulAbilitySystemLibrary::GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

bool USoulAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayer = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
	const bool bBothAreEnemy = FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy");
	const bool Friend = bBothArePlayer || bBothAreEnemy;
	return !Friend;

	//const bool FirstActorIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	//const bool SecondActorIsPlayer = SecondActor->ActorHasTag(FName("Player"));

	//return FirstActorIsPlayer != SecondActorIsPlayer;
}

int32 USoulAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);


	return static_cast<int32>(XPReward);
}
