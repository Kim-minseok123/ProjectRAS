// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RASPlayerState.h"
#include "InputActionValue.h"
#include "RASCombatComponent.generated.h"

/**
 * URASCombatComponent
 * - 플레이어의 전투 관련 로직을 관리하는 컴포넌트입니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRAS_API URASCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URASCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 전투 모드 On/Off 설정, 애니메이션에 영향
	void SetInBattle(bool InBattle) { bInBattle = InBattle; }
	bool GetInBattle() const { return bInBattle; }
	void SetInBattleTimer();

	void Roll(const FInputActionValue& Value);
	void PressTab();
	void PressComboAction();
	void PressShift();
	void PressShiftEnd();
	void PressF();
	void PressQ();
	void PressE();
	void PressRightClick();
	void PressRightClickHold();
	void PressRightClickEnd();


	// 특정 대상으로 락온 설정
	void SetLockedOnTarget(class ARASCharacterBase* Target);
	// 락온 대상 순환 변경
	void CycleLockOnTarget();
	// 현재 락온 대상을 반환
	class ARASCharacterBase* GetLockedOnTarget() const { return LockOnTarget; }

	// 공격 등으로 피해를 입었을 때 호출
	void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage);
	// 타겟을 처형할 때 호출
	void KillTarget(class ARASCharacterBase* Target);

	// 전투 상태를 설정 및 확인합니다.
	void SetCombatState(EPlayerCombatState NewState) { CombatState = NewState; }
	EPlayerCombatState GetCombatState() const { return CombatState; }

	// 전투 종료(죽음) 처리
	void Death();

	// 포션 개수 회복
	void RecoverPotion();

	// 포션 사용
	void UsePotion();
	
protected:
	virtual void BeginPlay() override;

	// 범위 내의 모든 적을 탐색합니다.
	void FindAllEnemyInRange();

	// 가장 가까운 적을 락온 대상으로 설정합니다.
	void SetClosestLockedOnTarget();

protected:
	// 현재 전투 상태 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	EPlayerCombatState CombatState;

	// 범위 내의 적들을 저장하는 집합
	TSet<TObjectPtr<class ARASCharacterBase>> TargetEnemys;

	// 현재 락온 대상
	TObjectPtr<class ARASCharacterBase> LockOnTarget;

	// 패링 확인 변수
	float ParryingTime = 0.f;

	// 이 컴포넌트를 소유한 플레이어 참조
	TObjectPtr<class ARASPlayer> OwnerPlayer;

	// 전투 모드 관련 타이머와 상태 변수
	FTimerHandle BattleTimer;
	bool bInBattle = false;
	bool bIsPressShift = false;

	FTimerHandle SkillETimer;
	FTimerHandle SkillQTimer;

	// 회복
	int32 PotionCount = 5;
	FTimerHandle PotionTimer;

};
