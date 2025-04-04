// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "InputActionValue.h"
#include "Data/RASPlayerState.h"
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
public:
	void PressTab();
protected:
	FVector2D LastMoveInput;
public:
	void LockOn();
	void LockOff();
// Camera Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> ExecuteCamera;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACameraActor> ExecuteCameraActor;

	TObjectPtr<class AActor> OriginalCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> HeadPoint;
public:
	void SwitchToExecutionCamera(float BlendTime);
	void SwitchBackToOriginalCamera(float BlendTime);

// Battle Section
public:
	void SetCombatState(EPlayerCombatState InState) { CombatState = InState; }
	EPlayerCombatState GetCombatState() const { return CombatState; }

	void SetInBattle(bool InBattle) { bInBattle = InBattle; }
	bool GetInBattle() const { return bInBattle; }
	void SetInBattleTimer();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EPlayerCombatState CombatState;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	uint8 bLockOn : 1;

	bool bIsPressShift = false;
	FTimerHandle BattleTimer;
	bool bInBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASComboComponent> ComboAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ParryingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ExecuteMontage;

	void PressComboAction();
	void PressShift();
	void PressShiftEnd();
	void PressF();
	void PressQ();
	void PressE();
	void PressRightClick();
	void PressRightClickEnd();

	void FindAllEnemyInRange();
	void SetClosestLockedOnTarget();
	TSet<TObjectPtr<ARASCharacterBase>> TargetEnemys;
	TObjectPtr<ARASCharacterBase> LockOnTarget;

	float ParryingTime = 0.f;
public:
	void SetLockedOnTarget(ARASCharacterBase* Target);
	void CycleLockOnTarget();
	class ARASCharacterBase* GetLockedOnTarget() const { return LockOnTarget; }
	virtual void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage) override;

protected:
	void KillTarget(ARASCharacterBase* Target);
// AnimMontage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	int rollTime = 1;

// Widget Section
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASPlayerHUDWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class URASPlayerHUDWidget> PlayerHUDWidget;

// Death Section
public:
	virtual void Death() override;
};
