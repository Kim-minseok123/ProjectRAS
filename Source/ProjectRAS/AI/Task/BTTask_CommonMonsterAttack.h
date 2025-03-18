// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CommonMonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_CommonMonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_CommonMonsterAttack();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	UFUNCTION()
	void EndTask();
};
