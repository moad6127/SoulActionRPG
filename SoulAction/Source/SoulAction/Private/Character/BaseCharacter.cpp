// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "SoulAction/SoulAction.h"
#include "SoulGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	LockOnPosision = CreateDefaultSubobject<USceneComponent>(TEXT("LockOnPosision"));
	LockOnPosision->SetupAttachment(GetRootComponent());

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, EquippedWeapon);
}

FVector ABaseCharacter::GetLockOnPosisionLocation() const
{
	
	return LockOnPosision->GetComponentLocation();
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::Die()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Unequip(this);
	}
	MulticastHandleDeath();

}

bool ABaseCharacter::bInitWeaponGet_Implementation() const
{
	return bInitWeapon;
}

void ABaseCharacter::StartDash_Implementation()
{
}

void ABaseCharacter::EndDash_Implementation()
{
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation() 
{
	return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontage_Implementation()
{
	return AttackMontage;
}

UNiagaraSystem* ABaseCharacter::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontage)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

FVector ABaseCharacter::GetMovementDirection_Implementation()
{
	FVector InputDir = GetLastMovementInputVector();
	if (!InputDir.IsNearlyZero())
	{
		return InputDir.GetSafeNormal();
	}
	return GetActorForwardVector();
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

TArray<TSubclassOf<UGameplayAbility>> ABaseCharacter::GetGrantedAbilitiesFromEquipWeapon_Implementation() const
{
	if (EquippedWeapon)
	{
		return EquippedWeapon->GetGrantedAbilities();
	}
	return TArray<TSubclassOf<UGameplayAbility>>();
}

FName ABaseCharacter::GetEquippedWeaponName_Implementation() const
{
	if (EquippedWeapon)
	{
		return EquippedWeapon->GetWeaponName();
	}
	return FName();
}


void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	Dissolve();
	OnDied.Broadcast();
	bDead = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayeffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayeffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayeffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());

}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::InitStartWeapon(const FGameplayTag& WeaponTag)
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (SoulASC)
	{
		SoulASC->InitEquipWeapon(WeaponTag);
		bInitWeapon = false;
	}
}

void ABaseCharacter::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}

	SoulASC->AddCharacterAbilities(Abilities);
}

void ABaseCharacter::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	USoulAbilitySystemComponent* SoulASC = CastChecked<USoulAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}
	SoulASC->AddCharacterPassiveAbilities(Abilities);
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	if (MontageTag.MatchesTagExact(SoulGameplayTags::CombatSocket_Weapon) && IsValid(EquippedWeapon))
	{
		return EquippedWeapon->GetTipSocketLocation();
	}

	if (MontageTag.MatchesTagExact(SoulGameplayTags::CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	if (MontageTag.MatchesTagExact(SoulGameplayTags::CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(SoulGameplayTags::CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	

	return FVector();
}

void ABaseCharacter::Equip_Implementation(ABaseWeapon* Weapon)
{
	if (HasAuthority())
	{
		EquipWeapon(Weapon);
	}
}

void ABaseCharacter::EquipWeapon(ABaseWeapon* WeaponToEquip)
{

	if (WeaponToEquip)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Unequip(this);
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->Equip(this);

		AttachEquippedWeapon();
		//UE_LOG(LogTemp, Warning, TEXT("EquippedWeapon: %s"), *EquippedWeapon->GetName());
	}
}

void ABaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterialInst))
	{
		UMaterialInstanceDynamic* DynamicInst = UMaterialInstanceDynamic::Create(DissolveMaterialInst, this);
		GetMesh()->SetMaterial(0, DynamicInst);

		StartDissolveTimeline(DynamicInst);
	}
}

void ABaseCharacter::OnRep_EquippedWeapon()
{
	if (EquippedWeapon)
	{
		AttachEquippedWeapon();
	}
}

void ABaseCharacter::AttachEquippedWeapon()
{
	if (EquippedWeapon)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		EquippedWeapon->AttachToComponent(GetMesh(),
			AttachRules,
			EquippedWeapon->GetAttachWeaponSocketName());
		if (USkeletalMeshComponent* WeaponMesh = EquippedWeapon->GetWeaponMesh())
		{
			if (UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance())
			{
				AnimInstance->InitializeAnimation();
			}
		}
	}
}

