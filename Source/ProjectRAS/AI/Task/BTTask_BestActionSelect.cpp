// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BestActionSelect.h"
#include "Interface/Monster/Boss/RASBossInfoInterface.h"
#include "Character/RASCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"
#include "Data/RASBossScoreData.h"
#include "AIController.h"


UBTTask_BestActionSelect::UBTTask_BestActionSelect()
{

}

EBTNodeResult::Type UBTTask_BestActionSelect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	ARASCharacterBase* Boss = Cast<ARASCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	ARASCharacterBase* Target = Cast<ARASCharacterBase>(BlackboardComp->GetValueAsObject(BBTarget));
	if (!Target)  return EBTNodeResult::Failed;

	IRASBossInfoInterface* BossInfo = Cast<IRASBossInfoInterface>(Boss);
	if (!BossInfo)  return EBTNodeResult::Failed;

	int32 MaxIndex = BossInfo->GetSkillScoreDataCount();

	const float Now = Boss->GetWorld()->GetTimeSeconds();
	const float BossHpPct = BossInfo->GetHealthPercent();
	const float BossStaminaPct = BossInfo->GetStaminaPercent();
	const float Distance = FVector::Distance(Boss->GetActorLocation(), Target->GetActorLocation());

	int32 BestIdx = BlackboardComp->GetValueAsInt(BBBestSkillIndex);
	if (BestIdx != -1)  return EBTNodeResult::Failed;

	TArray<FCandidate> Candidates;    
	float MaxScore = -FLT_MAX;

	for (int32 i = 1; i <= MaxIndex; ++i)
	{
		FSkillScoreData& Skill = BossInfo->GetSkillScoreData(i);
		if (Now - Skill.LastUsedTime < Skill.Cooldown) continue;

		const float DistScore = 1.f - FMath::Abs(Distance - Skill.IdealRange) / Skill.IdealRange;
		const float HPScore = 1.f - BossHpPct;
		const float StaminaScore = 1.f - BossStaminaPct;
		const float Noise = FMath::FRandRange(0.f, 0.7f);

		float Score = Skill.BaseWeight
			+ Skill.DistanceWeight * DistScore
			+ Skill.HpWeight * HPScore
			+ Skill.StaminaWeight * StaminaScore
			+ Noise;

		if (LastSkillIndex == i) Score -= RepeatPenalty;

		MaxScore = FMath::Max(MaxScore, Score);
		Candidates.Add({ i, Score });
	}

	// 상위 25퍼 필터링
	const float Threshold = MaxScore * 0.75f;
	float TotalWeight = 0.f;

	for (const FCandidate& C : Candidates)
	{
		if (C.Score >= Threshold)
		{
			TotalWeight += C.Score;
			UE_LOG(LogTemp, Log, TEXT("상위 25퍼에 든 스킬 %d"), C.Index);
		}
	}

	// Pick
	float Pick = FMath::FRandRange(0.f, TotalWeight);
	for (const FCandidate& C : Candidates)
	{
		if (C.Score >= Threshold)
		{
			Pick -= C.Score;
			if (Pick <= 0.f)
			{
				BestIdx = C.Index;
				break;
			}
		}
	}
	BlackboardComp->SetValueAsInt(BBBestSkillIndex, BestIdx);

	LastSkillIndex = BestIdx;
	UE_LOG(LogTemp, Log, TEXT("%d 선택"), BestIdx);

	return Result;
}
