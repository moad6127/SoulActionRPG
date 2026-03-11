// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SoulDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemLibrary.h"

void USoulDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return;
	}
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}

	FDamageEffectParams DamageEffectParams = MakeDamageEffectPrarmsFromClassDefaults(TargetActor);

	FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
	Rotation.Pitch = 45.f;
	const FVector ToTarget = Rotation.Vector();
	DamageEffectParams.DeathImpulse = ToTarget * DamageEffectParams.DeathImpulseMagnitude;
	const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
	if (bKnockback)
	{
		DamageEffectParams.KnockbackForce = ToTarget * DamageEffectParams.KnockbackForceMagnitude;
	}
	USoulAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

	/*
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	*/
}

FDamageEffectParams USoulDamageGameplayAbility::MakeDamageEffectPrarmsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComp = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;

	return Params;
}

float USoulDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage USoulDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontage) const
{
	if (TaggedMontage.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontage.Num() - 1);
		return TaggedMontage[Selection];
	}
	return FTaggedMontage();
}

