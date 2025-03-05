// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RASBattleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URASBattleInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FCharacterAttackFinished)

/**
 * 
 */
class PROJECTRAS_API IRASBattleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished) = 0;

	virtual void StartAttackMontage(int InAttackNumber = 0) = 0;

	virtual void AttackTo(int InAttackNumber = 0) = 0;

	virtual void EndAttack() = 0;
};
