// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "Interface/Monster/RASMonsterInfoInterface.h"
#include "RASMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASMonster : public ARASCharacterBase, public IRASMonsterInfoInterface
{
	GENERATED_BODY()
public:
	ARASMonster();

	virtual void PostInitializeComponents() override;

	virtual class ARASCharacterBase* GetTarget() override;

	virtual void SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished) override;

	virtual void StartAttackMontage(int InAttackNumber = 0) override;

	virtual void EndAttack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage; 

	UPROPERTY()
	TObjectPtr<class ARASCharacterBase> Target; 

	FCharacterAttackFinished OnAttackFinished;

	bool bUnflinching = false;
};
