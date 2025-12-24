

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
#include "SoulAction/SoulAction.h"
#include "Net/UnrealNetwork.h"

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

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	CharacterClass = ECharacterClass::Elementalist;
}

void ASoulCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASoulCharacter, bTargetLockOn);
	DOREPLIFETIME(ASoulCharacter, TargetActor);

}


void ASoulCharacter::ToggleTargetLock()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	bTargetLockOn = !bTargetLockOn;

	if (bTargetLockOn)
	{
		FindLockOnTarget();
		ServerToggleTargetLock(TargetActor);
		if (TargetActor == nullptr)
		{
			bTargetLockOn = false;
		}
	}
	else
	{
		ServerToggleTargetLock(nullptr);
	}

	TargetLockOnMovementSetting();

}

void ASoulCharacter::ServerToggleTargetLock_Implementation(ABaseCharacter* RequestedTarget)
{
	if (RequestedTarget && RequestedTarget->IsAlive())
	{
		if (TargetActor)
		{
			TargetActor->OnDied.RemoveDynamic(this, &ASoulCharacter::OnTargetDied);
		}
		TargetActor = RequestedTarget;
		bTargetLockOn = true;

		TargetActor->OnDied.AddDynamic(this, &ASoulCharacter::OnTargetDied);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;

	}
	else
	{
		if (TargetActor)
		{
			TargetActor->OnDied.RemoveDynamic(this, &ASoulCharacter::OnTargetDied);
		}
		TargetActor = nullptr;
		bTargetLockOn = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}

	/*
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
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else
	{
		if (TargetActor)
		{
			TargetActor->OnDied.RemoveDynamic(this, &ASoulCharacter::OnTargetDied);
		}
		TargetActor = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	*/

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




	//TODO : 나중에 DrawDebug지우기
	/*Draw Debug Capsule*/
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
	/*Draw Debug Capsule*/

	FCollisionShape Sphere = FCollisionShape::MakeSphere(LockOnSphereRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신 무시

	TArray<FHitResult> Hits;
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

		//시야 방향체크
        float Dot = FVector::DotProduct(CrosshairWorldDirection.GetSafeNormal(), ToActor.GetSafeNormal());
        if (Dot < 0.2f) continue; // 0.2 ~ 0.9 사이로 조절 가능
		
		//장애물 체크
        FHitResult LoSHit;
        FCollisionQueryParams LoSParams;
        LoSParams.AddIgnoredActor(this);

        bool bBlocked = GetWorld()->LineTraceSingleByChannel(LoSHit, CrosshairWorldPosition, HitActor->GetActorLocation(), ECC_Visibility, LoSParams);
        if (bBlocked && LoSHit.GetActor() != HitActor)
        {
            continue;
        }

		//중앙과의 거리를 체크한후 가장가까운 Actor선택하도록 만들기
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
	if (!IsLocallyControlled())
	{
		return;
	}

	if (!bTargetLockOn || !TargetActor)
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	FVector TargetLocation = TargetActor->GetLockOnPosisionLocation();
	FVector MyLocation = ViewCamera->GetComponentLocation();

	FVector Direction = (TargetLocation - MyLocation);
	//Direction.Z = 0; // 평면 회전만 (YAW 회전)

	if (Direction.IsNearlyZero()) return;

	FRotator LookAtRotation = Direction.Rotation();

	//카메라 회전
	FRotator CurrentCtrlRot = PC->GetControlRotation();
	FRotator NewCtrlRot = FMath::RInterpTo(CurrentCtrlRot, LookAtRotation, DeltaTime, CameraInterpSpeed);
	PC->SetControlRotation(NewCtrlRot);

	//캐릭터 회전
	FRotator TargetYawRotation(0.f, LookAtRotation.Yaw, 0.f);
	FRotator NewActorRoation = FMath::RInterpTo(GetActorRotation(), TargetYawRotation, DeltaTime, CharacterInterpSpeed);
	SetActorRotation(NewActorRoation);
}

void ASoulCharacter::OnTargetDied()
{
	TargetActor = nullptr;
	bTargetLockOn = false;
	TargetLockOnMovementSetting();
}

void ASoulCharacter::OnRep_bTargeting()
{
	TargetLockOnMovementSetting();
}

void ASoulCharacter::OnRep_TargetActor()
{
	TargetLockOnMovementSetting();
}

void ASoulCharacter::TargetLockOnMovementSetting()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (bTargetLockOn && TargetActor)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
	if (ASoulController* SoulPlayerController = Cast<ASoulController>(GetController()))
	{
		SoulPlayerController->OnLockOnChanged.Broadcast(TargetActor);
	}
}


void ASoulCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Emplace(ACTOR_TAG_PLAYER);
}

void ASoulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		if (TargetActor && !GetAbilitySystemComponent()->HasMatchingGameplayTag(SoulGameplayTags::Abilities_Rolling))
		{
			UpdateLockOnCamera(DeltaTime);
			UE_LOG(LogTemp, Warning, TEXT("Time : %lf"),GetWorld()->GetTimeSeconds());
		}
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
	AddCharacterAbilities(StartupAbilities);
	AddCharacterPassiveAbilities(StartupPassiveAbilities);

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

