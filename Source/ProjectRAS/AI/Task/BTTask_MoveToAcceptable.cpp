// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveToAcceptable.h"
#include "Interface/Monster/Boss/RASBossInfoInterface.h"
#include "Character/RASCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"
#include "Data/RASBossScoreData.h"
#include "AIController.h"
#include "Character/Monster/Boss/RASBossMonster.h"
#include "Interface/RASBattleInterface.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBTTask_MoveToAcceptable::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	
	ARASBossMonster* Boss = Cast<ARASBossMonster>(OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr);
	IRASBossInfoInterface* BossInfo = Cast<IRASBossInfoInterface>(Boss);

	if (!Boss || !BossInfo ) return EBTNodeResult::Failed;

	int Idx = BB->GetValueAsInt(BBBestSkillIndex);
	if (Idx <= 0) return EBTNodeResult::Failed;
	FSkillScoreData& Skill = BossInfo->GetSkillScoreData(Idx);

    AcceptableRadius = Skill.IdealRange;
    return Super::ExecuteTask(OwnerComp, NodeMemory);
}
