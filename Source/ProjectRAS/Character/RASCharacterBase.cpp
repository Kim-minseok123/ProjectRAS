// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RASCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Stat/RASStatComponent.h"

// Sets default values
ARASCharacterBase::ARASCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(36.f, 83.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RASCapsule"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->SetWalkableFloorAngle(35.f);
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	Stat = CreateDefaultSubobject<URASStatComponent>(TEXT("Stats"));

	CreatureName = TEXT("UnKnown");
}

void ARASCharacterBase::SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished)
{
}

void ARASCharacterBase::StartAttackMontage(int InAttackNumber /*= 0*/)
{
}

void ARASCharacterBase::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{

}

void ARASCharacterBase::EndAttack()
{

}

void ARASCharacterBase::KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower)
{
    if (!Direction.IsNearlyZero())
    {
        Direction.Normalize();

        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(HitMontage);
            AnimInstance->Montage_JumpToSection(TEXT("Knockback"));
            AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
        }

		FVector KnockbackForce = Direction * InPower;

		// If using root motion, directly add impulse to the character's movement component
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->AddImpulse(KnockbackForce, true);
		}

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AnimInstance]()
            {
                if (AnimInstance)
                {
                    AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
                }
            }, 0.2f, false);
    }
}

void ARASCharacterBase::Death()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr) return;

	AnimInstance->OnMontageEnded.RemoveAll(this);
	AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);

	int DeathType = FMath::RandRange(0, 1);
	// front
	if (DeathType == 0)
	{
		if (DeathMontage == nullptr) return;

		float PlayResult = AnimInstance->Montage_Play(DeathMontage);

		AnimInstance->Montage_JumpToSection(TEXT("FrontDeath"));
	}
	// back
	else if (DeathType == 1)
	{
		if (DeathMontage == nullptr) return;

		float PlayResult = AnimInstance->Montage_Play(DeathMontage);

		AnimInstance->Montage_JumpToSection(TEXT("BackDeath"));
	}
}

