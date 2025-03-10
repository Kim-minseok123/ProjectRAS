// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "InputActionValue.h"
#include "RASPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASPlayer : public ARASCharacterBase
{
	GENERATED_BODY()
public:
	ARASPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
// Input Section
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LeftAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShiftAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ParryingAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MappingContext;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);

	FVector2D LastMoveInput;
public:
	bool bIsRolling = false;
	void LockOn();

// Camera Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Battle Section
public:
	void SetIsAttacking(bool InIsAttacking) { bIsAttacking = InIsAttacking; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	uint8 bLockOn : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	uint8 bIsAttacking : 1;

	bool bIsPressShift = false;
	bool bIsPressF = false;
	bool bIsParrying = false;
	bool bIsSkilling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASComboComponent> ComboAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ParryingMontage;

	void PressComboAction();
	void PressShift();
	void PressShiftEnd();
	void PressF();
	void PressFEnd();
	void PressQ();
	void PressE();
	void PressRightClick();
	void PressRightClickEnd();

public:
	virtual void HitFromActor(class ARASCharacterBase* InFrom, int InDamage) override;
// AnimMontage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	int rollTime = 1;
};
