// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster/Common/RASCommonMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "Character/Monster/Common/RASCommonMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

URASCommonMonsterAnimInstance::URASCommonMonsterAnimInstance()
{

}

void URASCommonMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ARASCommonMonster>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URASCommonMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
