// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "SoulGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/SoulAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "SoulAbilityTypes.h"

struct SoulDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Block);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	SoulDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Block, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, CriticalHitDamage, Source, false);
	}

};

static const SoulDamageStatics& DamageStatics()
{
	static SoulDamageStatics DStatics;

	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParmas;
	EvaluationParmas.SourceTags = SourceTags;
	EvaluationParmas.TargetTags = TargetTags;

	/*
	* SetByCaller로 Damage 가져오기
	*/
	float Damage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::Damage);

	/*
	* Block관련 
	*/

	float TargetBlock = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockDef, EvaluationParmas, TargetBlock);
	TargetBlock = FMath::Max<float>(TargetBlock, 0.f);

	/*
	* Block되면 데미지 반절로
	*/
	const bool bBlock = FMath::RandRange(1, 100) < TargetBlock;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	USoulAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlock);

	Damage = bBlock ? Damage / 2.f : Damage;

	const UCharacterClassInfo* CharacterClassInfo = USoulAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	/*
	* Armor와 Armor관통
	*/
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParmas, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParmas, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficients) / 100.f;
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficients = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	Damage *= (100 - EffectiveArmor* EffectiveArmorCoefficients) / 100.f;

	/*
	* 치명타 관련
	*/
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParmas, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0, SourceCriticalHitChance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParmas, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0, SourceCriticalHitDamage);

	const bool bCriticalHit = FMath::RandRange(0, 100) < SourceCriticalHitChance;
	USoulAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	Damage = bCriticalHit ? (2.f + SourceCriticalHitDamage * 0.01f) * Damage : Damage;

	FGameplayModifierEvaluatedData EvaluatedData(USoulAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
