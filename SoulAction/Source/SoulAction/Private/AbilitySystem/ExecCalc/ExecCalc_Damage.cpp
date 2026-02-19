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

	int32 SourcePlayerLevel = 1;
	int32 TargetPlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	//ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	//ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParmas;
	EvaluationParmas.SourceTags = SourceTags;
	EvaluationParmas.TargetTags = TargetTags;


	/*
	* Debuff
	*/

	for (TTuple<FGameplayTag, FGameplayTag> Pair : SoulGameplayTags::DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(SoulGameplayTags::Debuff_Params_Chance, false, -1.f);
		
			//속성별 저항이 따로 존재할경우 계산한후 Resistance자리에 넣기
			float Resistance = 10.f;
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - Resistance) / 100;
			const int32 RandNums = FMath::RandRange(1, 100);
			const bool bDebuff = RandNums < EffectiveDebuffChance;
			//UE_LOG(LogTemp, Warning, TEXT("DebuffChance : %d"), RandNums);
			//UE_LOG(LogTemp, Warning, TEXT("DebuffChance : %f"), EffectiveDebuffChance);
			if (bDebuff)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Debuff"));
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				USoulAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				USoulAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::Debuff_Params_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(SoulGameplayTags::Debuff_Params_Durtion, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(SoulGameplayTags::Debuff_Params_Frequency, false, -1.f);

				USoulAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				USoulAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				USoulAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}

	}

	/*
	* SetByCaller로 Damage 가져오기
	*/
	float Damage = 0.f;
	for (FGameplayTag DamageTypeTag : SoulGameplayTags::DamageTypes)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag,false);
		Damage += DamageTypeValue;
	}
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
	const float ArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
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
	const float EffectiveArmorCoefficients = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
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
