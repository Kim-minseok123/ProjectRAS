// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_IsCanDash.h"
#include "Interface/Monster/RASMonsterInfoInterface.h"
#include "AIController.h"
#include "Character/RASCharacterBase.h"
#include "Utils/RASBlackBoardKey.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsCanDash::UBTService_IsCanDash()
{

}

void UBTService_IsCanDash::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	if (Distance <= DashDistance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCanDash, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCanDash, false);
	}
	return;
}
