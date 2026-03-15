

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
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "DrawDebugHelpers.h"



ASoulCharacter::ASoulCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	LevelUpNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComp");
	LevelUpNiagaraComp->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComp->bAutoActivate = false;

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

	//가까울때 회전 못하도록 막기

	FVector ToTarget = TargetActor->GetActorLocation() - GetActorLocation();
	if (ToTarget.SizeSquared() < FMath::Square(100.f))
	{
		return;
	}

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
	if (HasAuthority())
	{
		TargetActor = nullptr;
		bTargetLockOn = false;
	}

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

	if (!IsLocallyControlled())
	{
		return;
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

void ASoulCharacter::OnRep_Stunned()
{
	if (USoulAbilitySystemComponent* SoulASC = Cast<USoulAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(SoulGameplayTags::Player_Block_InputHeld);
		BlockedTags.AddTag(SoulGameplayTags::Player_Block_InputPressed);
		BlockedTags.AddTag(SoulGameplayTags::Player_Block_InputReleased);

		if (bIsStunned)
		{
			SoulASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComp->Activate();
		}
		else
		{
			SoulASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComp->Deactivate();
		}
	}
}

void ASoulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		if (TargetActor && !GetAbilitySystemComponent()->HasMatchingGameplayTag(SoulGameplayTags::Abilities_Rolling))
		{
			UpdateLockOnCamera(DeltaTime);
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
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	
	AbilitySystemComponent->RegisterGameplayTagEvent(SoulGameplayTags::Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ASoulCharacter::StunTagChanged);
	InitializeDefaultAttributes();


	if (ASoulController* SoulPlayerController = Cast<ASoulController>(GetController()))
	{
		if (ASoulHUD* SoulHUD = Cast<ASoulHUD>(SoulPlayerController->GetHUD()))
		{
			SoulHUD->InitOverlay(SoulPlayerController, SoulPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitStartWeapon(SoulGameplayTags::WeaponTag_Player_Staff);
}



void ASoulCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버용 Ability Actor 초기화 하기
	InitAbilityActorInfo();
	//AddCharacterAbilities(EquippedWeapon->GetGrantedAbilities());
	AddCharacterAbilities(StartupAbilities);
	AddCharacterPassiveAbilities(StartupPassiveAbilities);

}

void ASoulCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//클라이언트용 Actor초기화
	InitAbilityActorInfo();
}

void ASoulCharacter::AddToXP_Implementation(int32 InXP)
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);

	SoulPlayerState->AddToXP(InXP);
}

void ASoulCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}
void ASoulCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComp))
	{
		const FVector CameraLocation = ViewCamera->GetComponentLocation();
		const FVector NiagaraCompLocation = LevelUpNiagaraComp->GetComponentLocation();
		const FRotator ToCameraLotation = (CameraLocation - NiagaraCompLocation).Rotation();

		LevelUpNiagaraComp->SetWorldRotation(ToCameraLotation);
		LevelUpNiagaraComp->Activate(true);
	}
}

int32 ASoulCharacter::GetXP_Implementation() const
{
	const ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->GetXP();
}

int32 ASoulCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ASoulCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->LevelUpInfo->LevelUpInfomation[Level].AttributePointAward;
}

int32 ASoulCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->LevelUpInfo->LevelUpInfomation[Level].SpellPointAward;
}

void ASoulCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	SoulPlayerState->AddToLevel(InPlayerLevel);

	if (USoulAbilitySystemComponent* ASC = Cast<USoulAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->UpdateAbilityStatus(SoulPlayerState->GetPlayerLevel());
	}

}

void ASoulCharacter::AddToAttributePoints_Implementation(int32 InAttributePoins)
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	SoulPlayerState->AddToAttributePoints(InAttributePoins);
}

void ASoulCharacter::AddToSpellPoints_Implementation(int32 InSpellPoins)
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	SoulPlayerState->AddToSpellPoints(InSpellPoins);
}

int32 ASoulCharacter::GetAttributePoints_Implementation() const
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->GetAttributePoints();
}

int32 ASoulCharacter::GetSpellPoints_Implementation() const
{
	ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->GetSpellPoints();
}

int32 ASoulCharacter::GetPlayerLevel_Implementation()
{
	const ASoulPlayerState* SoulPlayerState = GetPlayerState<ASoulPlayerState>();
	check(SoulPlayerState);
	return SoulPlayerState->GetPlayerLevel();
}

