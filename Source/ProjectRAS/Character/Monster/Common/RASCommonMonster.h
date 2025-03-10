// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/RASMonster.h"
#include "RASCommonMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASCommonMonster : public ARASMonster
{
	GENERATED_BODY()
public:
	ARASCommonMonster();

	virtual void PostInitializeComponents() override;

	virtual void StartAttackMontage(int InAttackNumber = 0) override;

	virtual void EndAttack() override;

	virtual void HitFromActor(class ARASCharacterBase* InFrom, int InDamage) override;
};
