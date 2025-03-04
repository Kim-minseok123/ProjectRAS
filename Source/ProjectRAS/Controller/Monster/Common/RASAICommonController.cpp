// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Monster/Common/RASAICommonController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ARASAICommonController::ARASAICommonController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackBoardDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/1_ProjectRAS/AI/Common/BB_RASCommonMonster.BB_RASCommonMonster'"));
	if (BlackBoardDataRef.Object)
	{
		BBAsset = BlackBoardDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Script/AIModule.BehaviorTree'/Game/1_ProjectRAS/AI/Common/BT_RASCommonMonster.BT_RASCommonMonster'"));
	if (BehaviorTreeRef.Object)
	{
		BTAsset = BehaviorTreeRef.Object;
	}
}

void ARASAICommonController::RunAI()
{
	Super::RunAI();
}

void ARASAICommonController::StopAI()
{
	Super::StopAI();
}

void ARASAICommonController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void ARASAICommonController::SetupBlackBoard()
{
	Blackboard->SetValueAsVector(TEXT("HOME_POS"), GetPawn()->GetActorLocation());
}
