// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ResetAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"

UBTTask_ResetAttack::UBTTask_ResetAttack()
{

}

EBTNodeResult::Type UBTTask_ResetAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCooldown, false);

	return EBTNodeResult::Succeeded;
}
