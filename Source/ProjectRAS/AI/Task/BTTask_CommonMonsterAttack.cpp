// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_CommonMonsterAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/RASBattleInterface.h"
#include "AIController.h"
#include "Utils/RASBlackBoardKey.h"
#include "Character/RASCharacterBase.h"

UBTTask_CommonMonsterAttack::UBTTask_CommonMonsterAttack()
{

}

EBTNodeResult::Type UBTTask_CommonMonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return EBTNodeResult::Failed;

	IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(ControllingPawn);
	if (BattleInterface == nullptr)
		return EBTNodeResult::Failed;

	ARASCharacterBase* Target = Cast<ARASCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBTarget));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FCharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCooldown, true);

	BattleInterface->SetAttackFinishedDelegate(OnAttackFinished);
	BattleInterface->StartAttackMontage();

	return EBTNodeResult::InProgress;
}

