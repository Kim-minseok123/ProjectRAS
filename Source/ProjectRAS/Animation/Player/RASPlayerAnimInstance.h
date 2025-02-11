// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RASPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	URASPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class ARASPlayer> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 IsInAir : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Roll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	FRotator RotationLastTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 IsAccelerating : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 IsAttacking : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 FullBody : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float MoveDirectionX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float MoveDirectionY;
};
