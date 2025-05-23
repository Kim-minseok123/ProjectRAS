// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_BossActions.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_BossActions : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BossActions();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    EBTNodeResult::Type StartAttack();

    UFUNCTION()
    void OnAttackFinished();

    TObjectPtr<class UBlackboardComponent> BB;
    TObjectPtr<class UBehaviorTreeComponent> CachedOwnerComp;
    TObjectPtr<class AAIController> AI;
    TObjectPtr<class ARASBossMonster> Boss;
    TObjectPtr<class ARASCharacterBase> Target;
    int32           Idx = INDEX_NONE;


};
