// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RASPlayerState.h"
#include "InputActionValue.h"
#include "RASCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URASCombatComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EPlayerCombatState CombatState;

	// Called when the game starts
	virtual void BeginPlay() override;

	void FindAllEnemyInRange();
	void SetClosestLockedOnTarget();
	TSet<TObjectPtr<class ARASCharacterBase>> TargetEnemys;
	TObjectPtr<class ARASCharacterBase> LockOnTarget;

	float ParryingTime = 0.f;

	TObjectPtr<class ARASPlayer> OwnerPlayer;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetInBattle(bool InBattle) { bInBattle = InBattle; }
	bool GetInBattle() const { return bInBattle; }
	void SetInBattleTimer();

	bool bIsPressShift = false;
	FTimerHandle BattleTimer;
	bool bInBattle = false;

	void Roll(const FInputActionValue& Value);
	void PressTab();
	void PressComboAction();
	void PressShift();
	void PressShiftEnd();
	void PressF();
	void PressQ();
	void PressE();
	void PressRightClick();
	void PressRightClickEnd();

	void SetLockedOnTarget(ARASCharacterBase* Target);
	void CycleLockOnTarget();
	class ARASCharacterBase* GetLockedOnTarget() const { return LockOnTarget; }
	void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage);
	void KillTarget(ARASCharacterBase* Target);

	void SetCombatState(EPlayerCombatState NewState) { CombatState = NewState; }
	EPlayerCombatState GetCombatState() const { return CombatState; }
};
