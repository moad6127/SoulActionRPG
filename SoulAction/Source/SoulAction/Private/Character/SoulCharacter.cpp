

#include "Character/SoulCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/SoulPlayerState.h"
#include "Controller/SoulController.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "UI/HUD/SoulHUD.h"
#include "SoulGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BaseWeapon.h"

#include "DrawDebugHelpers.h"


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

void ASoulCharacter::ToggleTargetLock()
{
	bTargetLockOn = !bTargetLockOn;
	if (bTargetLockOn)
	{
		if (TargetActor)
		{
			TargetActor->OnDied.RemoveDynamic(this, &ASoulCharacter::OnTargetDied);
		}
		FindLockOnTarget();
		if (TargetActor == nullptr)
		{
			bTargetLockOn = false;
		}
		else
		{
			TargetActor->OnDied.AddDynamic(this, &ASoulCharacter::OnTargetDied);
		}
	}
	else
	{
		if (TargetActor)
		{
			TargetActor->OnDied.RemoveDynamic(this, &ASoulCharacter::OnTargetDied);
		}
		TargetActor = nullptr;
	}
}

void ASoulCharacter::FindLockOnTarget()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

	int32 ViewX = 0, ViewY = 0;
	PC->GetViewportSize(ViewX, ViewY);

	FVector2D CrosshairLocation(ViewX / 2.f, ViewY / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		PC,
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);


    FVector Start = CrosshairWorldPosition;
    FVector End = CrosshairWorldPosition + CrosshairWorldDirection * LockOnMaxRange;

    TArray<FHitResult> Hits;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신 무시

    FCollisionShape Sphere = FCollisionShape::MakeSphere(LockOnSphereRadius);

	FVector TraceCenter = (Start + End) * 0.5f;
	FVector TraceDir = (End - Start).GetSafeNormal();
	float HalfHeight = (End - Start).Size() * 0.5f;
	DrawDebugCapsule(
		GetWorld(),
		TraceCenter,
		HalfHeight,
		LockOnSphereRadius,
		FRotationMatrix::MakeFromZ(TraceDir).ToQuat(),
		FColor::Green,
		false,
		5.0f
	);


    bool bHit = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, Sphere, Params);

    if (!bHit || Hits.Num() == 0)
    {
        return;
    }

    AActor* BestActor = nullptr;
    float BestScreenDist = FLT_MAX;

    for (const FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor) continue;
        if (HitActor == this) continue;

        if (!HitActor->Implements<UCombatInterface>()) continue;

        FVector ToActor = HitActor->GetActorLocation() - CrosshairWorldPosition;
        float Dot = FVector::DotProduct(CrosshairWorldDirection.GetSafeNormal(), ToActor.GetSafeNormal());

        if (Dot < 0.2f) continue; // 0.2 ~ 0.9 사이로 조절 가능

        FHitResult LoSHit;
        FCollisionQueryParams LoSParams;
        LoSParams.AddIgnoredActor(this);

        bool bBlocked = GetWorld()->LineTraceSingleByChannel(LoSHit, CrosshairWorldPosition, HitActor->GetActorLocation(), ECC_Visibility, LoSParams);
        if (bBlocked && LoSHit.GetActor() != HitActor)
        {
            continue;
        }

        FVector2D ScreenPos;
        bool bProjected = PC->ProjectWorldLocationToScreen(HitActor->GetActorLocation(), ScreenPos);
        if (!bProjected) continue;

        float ScreenDist = FVector2D::Distance(ScreenPos, FVector2D(ViewX * 0.5f, ViewY * 0.5f));

        if (ScreenDist < BestScreenDist)
        {
            BestScreenDist = ScreenDist;
            BestActor = HitActor;
        }
    }

	TargetActor = Cast<ABaseCharacter>(BestActor);
}

void ASoulCharacter::UpdateLockOnCamera(float DeltaTime)
{
	if (!bTargetLockOn || !TargetActor)
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	FVector Direction = (TargetLocation - MyLocation);
	Direction.Z = 0; // 평면 회전만 (YAW 회전)

	if (Direction.IsNearlyZero()) return;

	FRotator LookAtRotation = Direction.Rotation();

	//TODO : 캐릭터도 타겟을 바라보도록 회전시키기

	FRotator CurrentCtrlRot = PC->GetControlRotation();
	float CameraInterpSpeed = 5.f;
	FRotator NewCtrlRot = FMath::RInterpTo(CurrentCtrlRot, LookAtRotation, DeltaTime, CameraInterpSpeed);
	PC->SetControlRotation(NewCtrlRot);
}

void ASoulCharacter::OnTargetDied()
{
	TargetActor = nullptr;
	bTargetLockOn = false;
}

void ASoulCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASoulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bTargetLockOn)
	{
		UpdateLockOnCamera(DeltaTime);
	}
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
	InitStartWeapon(SoulGameplayTags::WeaponTag_Player_Staff);
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
	AddCharacterAbilities(EquippedWeapon->GetGrantedAbilities());
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

