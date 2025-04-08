// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Monster/Common/RASAICommonController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"

ARASAICommonController::ARASAICommonController()
{
	
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
	Blackboard->SetValueAsVector(BBHomePos, GetPawn()->GetActorLocation());
}
