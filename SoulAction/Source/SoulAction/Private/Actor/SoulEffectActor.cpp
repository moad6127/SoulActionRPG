// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SoulEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ASoulEffectActor::ASoulEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void ASoulEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if (TargetASC == nullptr)
	{
		return;
	}
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void ASoulEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
}

void ASoulEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplycationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
}

