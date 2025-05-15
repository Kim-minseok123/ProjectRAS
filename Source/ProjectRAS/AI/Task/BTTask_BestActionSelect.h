// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BestActionSelect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_BestActionSelect : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BestActionSelect();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	int32 LastSkillIndex = -1;
	static constexpr float RepeatPenalty = 2.0f;
};
