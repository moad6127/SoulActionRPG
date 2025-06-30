// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoulCharacter.generated.h"

UCLASS()
class SOULACTION_API ASoulCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASoulCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:	

};
