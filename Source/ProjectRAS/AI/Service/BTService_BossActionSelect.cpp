// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_BossActionSelect.h"
#include "Interface/Monster/Boss/RASBossInfoInterface.h"
#include "Character/RASCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"
#include "Data/RASBossScoreData.h"
#include "AIController.h"

UBTService_BossActionSelect::UBTService_BossActionSelect()
{
	NodeName = TEXT("Calc Skill Utility");
	Interval = 0.25f; 
}

void UBTService_BossActionSelect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	ARASCharacterBase* Boss = Cast<ARASCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return;

	ARASCharacterBase* Target = Cast<ARASCharacterBase>(BlackboardComp->GetValueAsObject(BBTarget));
	if (!Target) return;

	IRASBossInfoInterface* BossInfo = Cast<IRASBossInfoInterface>(Boss);
	if (!BossInfo) return;

	

	int32 MaxIndex = BossInfo->GetSkillScoreDataCount();

	const float Now = Boss->GetWorld()->GetTimeSeconds();
	const float BossHpPct = BossInfo->GetHealthPercent();
	const float BossStaminaPct = BossInfo->GetStaminaPercent();
	const float Distance = FVector::Distance(Boss->GetActorLocation(), Target->GetActorLocation());

	float MaxScore = -1.f;
	int32 BestIdx = BlackboardComp->GetValueAsInt(BBBestSkillIndex);
	if (BestIdx != -1) return;

	for (int32 i = 1; i <= MaxIndex; i++)
	{
		FSkillScoreData& Skill = BossInfo->GetSkillScoreData(i);
		const bool bOnCD = Now - Skill.LastUsedTime < Skill.Cooldown;
		if (bOnCD) continue;

		const float DistScore = 1.f - FMath::Clamp(FMath::Abs(Distance - Skill.IdealRange) / Skill.IdealRange, 0.f, 1.f);
		const float HPScore = 1.f - BossHpPct;
		const float StaminaScore = 1.f - BossStaminaPct;
		const float RandomNoise = FMath::FRandRange(0.f, 0.15f);

		const float Score = Skill.BaseWeight + Skill.DistanceWeight * DistScore+
			Skill.HpWeight * HPScore + Skill.StaminaWeight * StaminaScore + RandomNoise;
		UE_LOG(LogTemp, Log, TEXT("%d의 Score : %f"), i, Score);

		if (Score > MaxScore)
		{
			MaxScore = Score;
			BestIdx = i;
		}
	}
	BlackboardComp->SetValueAsInt(BBBestSkillIndex, BestIdx);

	UE_LOG(LogTemp, Log, TEXT("%d 선택"), BestIdx);
}

