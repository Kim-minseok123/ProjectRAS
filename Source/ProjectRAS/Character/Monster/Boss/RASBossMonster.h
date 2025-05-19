// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Monster/RASMonster.h"
#include "Interface/Monster/Boss/RASBossInfoInterface.h"
#include "Data/RASBossScoreData.h"
#include "RASBossMonster.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnStopAttackDelegate);
/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASBossMonster : public ARASMonster, public IRASBossInfoInterface
{
	GENERATED_BODY()
	

public:
	ARASBossMonster();

	FOnStopAttackDelegate OnStopAttack;

	virtual void BeginPlay() override;
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
	void Test(int InAttackNumber = 0);
	TArray<FVector> GetWeaponPosition();
	FVector GetCircleAttackPosition();
	void SetWeaponOn(bool bWeaponOn);

	virtual FSkillScoreData& GetSkillScoreData(int32 InIdx) override;
	virtual int32 GetSkillScoreDataCount() override;
	virtual float GetHealthPercent() override;
	virtual float GetStaminaPercent() override;

	void InitUI();

	UFUNCTION(BlueprintCallable, Category="UI")
	bool ShowBossHUD();

	UFUNCTION(BlueprintCallable, Category="UI")
	bool HideBossHUD();
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

	UPROPERTY(EditAnywhere)
	TObjectPtr<class URASBossScoreData> BossScoreData;

	UPROPERTY(EditAnywhere)
	TMap<int, FSkillScoreData> SkillScoreDataMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASBossHUDWidget> BossHUDWidgetClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class URASBossHUDWidget> BossHUDWidget;
};
