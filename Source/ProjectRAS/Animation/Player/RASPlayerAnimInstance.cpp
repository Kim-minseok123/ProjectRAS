// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Player/RASPlayerAnimInstance.h"
#include "Character/Player/RASPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

URASPlayerAnimInstance::URASPlayerAnimInstance()
{

}

void URASPlayerAnimInstance::SetLockOn(bool InLockOn)
{
	bLockOn = InLockOn;
}

void URASPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<ARASPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void URASPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// IsValid
	if (Owner == nullptr) return;
	// Set IsInAir
	IsInAir = Movement->IsFalling();
	// Set Speed
	Velocity = Movement->Velocity;
	Speed = Velocity.Length();
	// Set Roll Pith Yaw
	{
		FRotator BAR = Owner->GetBaseAimRotation();
		FRotator CurrentRotation = Owner->GetActorRotation();
		FRotator Delta = (BAR - CurrentRotation).GetNormalized();
		Pitch = Delta.Pitch;
		Yaw = Delta.Yaw;
		Roll = Delta.Roll;
	}
	// Setting Yaw Delta for Leans
	{
		FRotator CurrentRotation = Owner->GetActorRotation();
		FRotator Delta = (RotationLastTick - CurrentRotation).GetNormalized();
		float TempYaw = Delta.Yaw;
		YawDelta = FMath::FInterpTo(YawDelta, ((TempYaw / DeltaSeconds) / 12), DeltaSeconds, 6.f);
		RotationLastTick = CurrentRotation;
	}
	// Set Accelerating
	{
		float Acclerate = Movement->GetCurrentAcceleration().Length();
		IsAccelerating = Acclerate > 0;
	}
	// Set IsFullBody
	{
		FullBody = GetCurveValue(TEXT("FullBody")) > 0;
	}

	if (!Velocity.IsNearlyZero())
	{
		// Velocity To Local Space
		FVector LocalMovementDirection = Owner->GetActorTransform().InverseTransformVector(Velocity.GetSafeNormal());

		// Set LocalMovementDirection X Y
		MoveDirectionY = LocalMovementDirection.Y;
		MoveDirectionX = LocalMovementDirection.X;

		//UE_LOG(LogTemp, Log, TEXT("Local Movement Direction: %s"), *LocalMovementDirection.ToString());
	}
}

