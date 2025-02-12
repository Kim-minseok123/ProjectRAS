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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MappingContext;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);
	void LockOn();

// Camera Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Battle Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	uint8 bLockOn : 1;


// AnimMontage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

	int rollTime = 1;
};
