// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/RASMonster.h"
#include "RASBossMonster.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnStopAttackDelegate);
/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASBossMonster : public ARASMonster
{
	GENERATED_BODY()
	

public:
	ARASBossMonster();

	FOnStopAttackDelegate OnStopAttack;

	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void StartAttackMontage(int InAttackNumber = 0) override;
	virtual void EndAttack() override;
	virtual void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage) override;
	virtual void KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower) override;
	virtual void Death() override;
	virtual void ExecuteDeath(int32 InDeathNumber) override;
	virtual void Dash(FVector InDireciton, float InDashSpeed) override;

	UFUNCTION(BlueprintCallable)
	void Test(int InAttackNumber = 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test"));
		StartAttackMontage(InAttackNumber);
	}
	TArray<FVector> GetWeaponPosition();
	FVector GetCircleAttackPosition(){ return WeaponCircleAttack->GetComponentLocation(); }
	void SetWeaponOn(bool bWeaponOn);
protected:

	bool bIsDeath = false;

	void PreDeath();

	bool bIsDashing = false;  

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> WeaponStart;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> WeaponEnd;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> WeaponCircleAttack;
};
