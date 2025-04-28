// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster/Boss/RASBossAnimInstance.h"
#include "Character/Monster/Boss/RASBossMonster.h"
#include "GameFramework/CharacterMovementComponent.h"

URASBossAnimInstance::URASBossAnimInstance()
{

}

void URASBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ARASBossMonster>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URASBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner == nullptr) return;

	IsInAir = Movement->IsFalling();

	Velocity = Movement->Velocity;
	Speed = Velocity.Length();

	{
		float Acclerate = Movement->GetCurrentAcceleration().Length();
		IsAccelerating = Acclerate > 0;
	}
}
