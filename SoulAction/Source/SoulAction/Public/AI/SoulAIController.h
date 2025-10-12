// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SoulAIController.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class SOULACTION_API ASoulAIController : public AAIController
{
	GENERATED_BODY()
public:

	ASoulAIController();

private:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
};
