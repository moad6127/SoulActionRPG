

#include "Character/SoulCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/SoulPlayerState.h"
#include "Controller/SoulController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "UI/HUD/SoulHUD.h"

ASoulCharacter::ASoulCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
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

void ASoulCharacter::InitAbilityActorInfo()
{

	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);

	SoulPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SoulPlayerState, this);
	AbilitySystemComponent = SoulPlayerState->GetAbilitySystemComponent();
	Cast<USoulAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = SoulPlayerState->GetAttributeSet();

	InitializeDefaultAttributes();

	if (ASoulController* SoulPlayerController = Cast<ASoulController>(GetController()))
	{
		if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(SoulPlayerController->GetHUD()))
		{
			SoulHUD->InitOverlay(SoulPlayerController, SoulPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

}

void ASoulCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버용 Ability Actor 초기화 하기
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ASoulCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//클라이언트용 Actor초기화
	InitAbilityActorInfo();
}

int32 ASoulCharacter::GetPlayerLevel()
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->GetPlayerLevel();
}

