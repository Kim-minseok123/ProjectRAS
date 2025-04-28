// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RASBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	URASBossAnimInstance();

	void SetHideWeapon(bool bInHide)
	{
		if(bInHide == true)
		{
			WeaponScale = FVector(0.0f, 0.0f, 0.0f);
		}
		else
		{
			WeaponScale = FVector(1.3f, 1.3f, 1.3f);
		}
	}

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class ARASBossMonster> Owner;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector WeaponScale = FVector(1.3f, 1.3f, 1.3f);
};
