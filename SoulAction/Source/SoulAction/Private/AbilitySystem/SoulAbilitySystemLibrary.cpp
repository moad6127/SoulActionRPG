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
#include "AbilitySystemBlueprintLibrary.h"
#include "SoulGameplayTags.h"

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

bool USoulAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float USoulAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float USoulAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float USoulAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag USoulAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (SoulEffectContext->GetDamageType().IsValid())
		{
			return *SoulEffectContext->GetDamageType();
		}
	}

	return FGameplayTag();
}

FVector USoulAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector USoulAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSoulGameplayEffectContext* SoulEffectContext = static_cast<const FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return SoulEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
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

void USoulAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void USoulAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetDebuffDamage(InDamage);
	}
}

void USoulAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetDebuffDuration(InDuration);
	}
}

void USoulAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void USoulAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		SoulEffectContext->SetDamageType(DamageType);
	}
}

void USoulAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpluse)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetDeathImpulse(InImpluse);
	}
}

void USoulAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FSoulGameplayEffectContext* SoulEffectContext = static_cast<FSoulGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SoulEffectContext->SetKnockbackForce(InForce);
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

void USoulAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	// 이중for문 사용
	/*
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0)
		{
			break;
		}
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PortentialTarget : ActorsToCheck)
		{
			const double Distance = (PortentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PortentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		NumTargetsFound++;
	}*/
	//Sort를 사용한 방법
	if (Actors.Num() == 0)
	{
		return;
	}
	TArray<AActor*> SortedActors = Actors;
	SortedActors.Sort([Origin](const AActor& A, const AActor& B) 
		{
			return FVector::DistSquared(A.GetActorLocation(), Origin) < FVector::DistSquared(B.GetActorLocation(), Origin);
		});

	const int32 NumTargts = FMath::Min(MaxTargets, SortedActors.Num());
	for (int32 i = 0; i < NumTargts; i++)
	{
		OutClosestTargets.Add(SortedActors[i]);
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

FGameplayEffectContextHandle USoulAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComp->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComp->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComp->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, SoulGameplayTags::Debuff_Params_Chance,DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, SoulGameplayTags::Debuff_Params_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, SoulGameplayTags::Debuff_Params_Durtion, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, SoulGameplayTags::Debuff_Params_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> USoulAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int i = 0; i < NumRotators;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}


	return Rotators;
}

TArray<FVector> USoulAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int i = 0; i < NumVectors;i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}


	return Vectors;
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
