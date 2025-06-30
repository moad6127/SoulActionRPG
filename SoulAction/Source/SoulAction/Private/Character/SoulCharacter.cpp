

#include "Character/SoulCharacter.h"

ASoulCharacter::ASoulCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASoulCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

