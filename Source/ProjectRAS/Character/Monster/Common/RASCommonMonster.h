// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/RASMonster.h"
#include "RASCommonMonster.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnStopAttackDelegate);
/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASCommonMonster : public ARASMonster
{
	GENERATED_BODY()
public:
	ARASCommonMonster();
	
	FOnStopAttackDelegate OnStopAttack;

	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;

	virtual void StartAttackMontage(int InAttackNumber = 0) override;

	virtual void EndAttack() override;

	virtual void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage) override;

	virtual void KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> StatusBarWidgetComponent;

	bool bIsDeath = false;

public:
	virtual void Death() override;

	virtual void ExecuteDeath(int32 InDeathNumber) override;
};
