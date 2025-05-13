// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToAcceptable.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_MoveToAcceptable : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
};
