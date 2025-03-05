// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ResetAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_ResetAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ResetAttack();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
