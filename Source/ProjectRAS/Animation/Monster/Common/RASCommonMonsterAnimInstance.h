// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RASCommonMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASCommonMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	URASCommonMonsterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class ARASCommonMonster> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 IsAccelerating : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	uint8 IsInAir : 1;
};
