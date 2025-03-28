// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_IsInAttackRange.h"
#include "AIController.h"
#include "Utils/RASBlackBoardKey.h"
#include "Character/RASCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/Monster/RASMonsterInfoInterface.h"
UBTService_IsInAttackRange::UBTService_IsInAttackRange()
{

}

void UBTService_IsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return;

	ARASCharacterBase* Target = Cast<ARASCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBTarget));
	if (Target == nullptr)
	{
		return;
	}
	IRASMonsterInfoInterface* MonsterInfo = Cast<IRASMonsterInfoInterface>(ControllingPawn);
	if (MonsterInfo == nullptr) return;
	ARASCharacterBase* MonsterToTarget = MonsterInfo->GetTarget();
	if (MonsterToTarget == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBTarget, nullptr);
		return;
	}
	float Distance = ControllingPawn->GetDistanceTo(Target);
	if (Distance <= 175.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCanAttack, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCanAttack, false);
	}
	return;
}
