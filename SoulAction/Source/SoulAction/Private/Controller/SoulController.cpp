// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SoulController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UI/HUD/SoulHUD.h"

void ASoulController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ShooterIMC, 0);
	}
}

void ASoulController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulController::Input_Move);
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulController::Input_Look);
	ShooterInputComponent->BindAction(AttributeMenu, ETriggerEvent::Completed, this, &ASoulController::ShowAttributeMenu);
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

void ASoulController::ShowAttributeMenu()
{
	if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(GetHUD()))
	{
		SoulHUD->ShowMenu();
	}
}

