// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SoulController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UI/HUD/SoulHUD.h"
#include "Input/SoulEnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "Character/SoulCharacter.h"
#include "UI/Widget/DamageTextComponent.h"


void ASoulController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamagetextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamagetextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount,bBlockedHit,bCriticalHit);
	}
}

void ASoulController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(SoulIMC, 0);
	}
}

void ASoulController::SetupInputComponent()
{
	Super::SetupInputComponent();

	USoulEnhancedInputComponent* SoulInputComponent = CastChecked<USoulEnhancedInputComponent>(InputComponent);
	SoulInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulController::Input_Move);
	SoulInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulController::Input_Look);
	SoulInputComponent->BindAction(AttributeMenu, ETriggerEvent::Completed, this, &ASoulController::ShowAttributeMenu);
	SoulInputComponent->BindAction(TargetLockAction, ETriggerEvent::Completed, this, &ASoulController::SetTargetLock);

	SoulInputComponent->BindAbilityAction(InputConfig, this, &ASoulController::AbilityInputTagPressed, &ASoulController::AbilityInputTagReleased, &ASoulController::AbilityInputTagHeld);

}

USoulAbilitySystemComponent* ASoulController::GetASC()
{
	if (SoulAbilitySystemComp == nullptr)
	{
		SoulAbilitySystemComp = Cast<USoulAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return SoulAbilitySystemComp;
}

void ASoulController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ASoulController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(-1 * InputAxisVector.Y);
}

void ASoulController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void ASoulController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ASoulController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagHeld(InputTag);
}

void ASoulController::ShowAttributeMenu()
{
	if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(GetHUD()))
	{
		SoulHUD->ShowMenu();
	}
}

void ASoulController::SetTargetLock()
{
	if (ASoulCharacter* SoulCharacter = Cast<ASoulCharacter>(GetPawn()))
	{
		SoulCharacter->ToggleTargetLock();
	}
}

