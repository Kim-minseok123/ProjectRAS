// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossActions.h"
#include "Interface/Monster/Boss/RASBossInfoInterface.h"
#include "Character/RASCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"
#include "Data/RASBossScoreData.h"
#include "AIController.h"
#include "Character/Monster/Boss/RASBossMonster.h"
#include "Interface/RASBattleInterface.h"
#include "Navigation/PathFollowingComponent.h"
#include "Animation/Monster/Boss/RASBossAnimInstance.h"

UBTTask_BossActions::UBTTask_BossActions()
{

}

EBTNodeResult::Type UBTTask_BossActions::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	AI = OwnerComp.GetAIOwner();
	Boss = Cast<ARASBossMonster>(AI ? AI->GetPawn() : nullptr);
	Battle = Cast<IRASBattleInterface>(Boss);
	BossInfo = Cast<IRASBossInfoInterface>(Boss);
	Target = Cast<ARASCharacterBase>(BB->GetValueAsObject(BBTarget));

	if (!Boss || !Battle || !BossInfo || !Target) return EBTNodeResult::Failed;

	Boss->OnStopAttack.RemoveAll(this);
	Boss->OnStopAttack.AddUObject(this, &UBTTask_BossActions::OnAttackFinished);

	Idx = BB->GetValueAsInt(BBBestSkillIndex);
	if(Idx <= 0) return EBTNodeResult::Failed;
	FSkillScoreData& Skill = BossInfo->GetSkillScoreData(Idx);

	BB->SetValueAsFloat(BBWaitTime, Skill.SkillDelay);

	return StartAttack();
}

EBTNodeResult::Type UBTTask_BossActions::StartAttack()
{
	FCharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			BB->SetValueAsInt(BBBestSkillIndex, -1);
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	);

	FVector LookVector = Target->GetActorLocation() - Boss->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Boss->SetActorRotation(FMath::RInterpTo(Boss->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 200.f));

	Battle->SetAttackFinishedDelegate(OnAttackFinished);

	Battle->StartAttackMontage(Idx);

	return EBTNodeResult::InProgress;
}

void UBTTask_BossActions::OnAttackFinished()
{
	if (URASBossAnimInstance* Anim = Cast<URASBossAnimInstance>(Boss->GetMesh()->GetAnimInstance()))
	{
		Anim->SetHideWeapon(false);
		Boss->SetWeaponOn(false);
	}
	BB->SetValueAsInt(BBBestSkillIndex, -1);
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}
