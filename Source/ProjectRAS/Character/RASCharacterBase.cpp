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
    
}

void ARASCharacterBase::Death()
{
    
}

void ARASCharacterBase::ExecuteDeath(int32 InDeathNumber)
{
    
}

float ARASCharacterBase::GetTotalStamina()
{
    return Stat->GetStamina();
}

