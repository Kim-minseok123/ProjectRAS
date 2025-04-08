// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_CommonMonsterAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/RASBattleInterface.h"
#include "AIController.h"
#include "Utils/RASBlackBoardKey.h"
#include "Character/RASCharacterBase.h"
#include "Character/Monster/Common/RASCommonMonster.h"

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

	CachedOwnerComp = &OwnerComp;
	ARASCommonMonster* CommonMonster = Cast<ARASCommonMonster>(ControllingPawn);
	if (CommonMonster == nullptr)
		return EBTNodeResult::Failed;
	CommonMonster->OnStopAttack.RemoveAll(this); 
	CommonMonster->OnStopAttack.AddUObject(this, &UBTTask_CommonMonsterAttack::EndTask);

	FCharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCooldown, true);

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 200.f));

	BattleInterface->SetAttackFinishedDelegate(OnAttackFinished);
	int AttackNumber = CommonMonster->GetMaxAttackNumber();
	if (AttackNumber > 0)
	{
		int RandomAttackNumber = FMath::RandRange(1, AttackNumber);
		BattleInterface->StartAttackMontage(RandomAttackNumber);
	}
	else
		BattleInterface->StartAttackMontage();



	return EBTNodeResult::InProgress;
}

void UBTTask_CommonMonsterAttack::EndTask()
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

