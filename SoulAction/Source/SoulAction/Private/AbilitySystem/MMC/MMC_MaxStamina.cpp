// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxStamina.h"
#include "AbilitySystem/SoulAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	VigorDef.AttributeToCapture = USoulAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);

	DexDef.AttributeToCapture = USoulAttributeSet::GetDexterityAttribute();
	DexDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DexDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	float Dex = 0.f;
	GetCapturedAttributeMagnitude(DexDef, Spec, EvaluationParameters, Dex);
	Dex = FMath::Max<float>(Dex, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();


	return 150 + 2.f * Vigor + 1.5 * Dex + 5.f * PlayerLevel;

}
