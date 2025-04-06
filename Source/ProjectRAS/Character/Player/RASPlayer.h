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
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:
	class URASCombatComponent* GetCombatComponent() const { return CombatComponent; }
	class URASComboComponent* GetComboComponent() const { return ComboAttack; }
	class URASPlayerAnimComponent* GetAnimComponent() { return PlayerAnimComponent; }
	class URASInputComponent* GetInputComponent() { return PlayerInput; }
	FVector2D GetLastMoveInput() const { return LastMoveInput; }

protected:
	FVector2D LastMoveInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASCombatComponent> CombatComponent;

	TObjectPtr<class URASInputComponent> PlayerInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASComboComponent> ComboAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class URASPlayerAnimComponent> PlayerAnimComponent;

public:
	void LockOn();
	void LockOff();
// Camera Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACameraActor> ExecuteCameraActor;

	TObjectPtr<class AActor> OriginalCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> HeadPoint;
public:
	void SwitchToExecutionCamera(float BlendTime);
	void SwitchBackToOriginalCamera(float BlendTime);

public:
	virtual void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASPlayerHUDWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class URASPlayerHUDWidget> PlayerHUDWidget;

public:
	virtual void Death() override;
};
