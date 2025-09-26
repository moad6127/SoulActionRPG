// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "SoulGameplayTags.h"

struct SoulDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Block);
	SoulDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Block, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, ArmorPenetration, Source, false);
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
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

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
	Damage = bBlock ? Damage / 2.f : Damage;

	/*
	* Armor와 Armor관통
	*/
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParmas, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParmas, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * 0.25f) / 100.f;
	Damage *= (100 - EffectiveArmor* 0.333f) / 100.f;


	FGameplayModifierEvaluatedData EvaluatedData(USoulAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
