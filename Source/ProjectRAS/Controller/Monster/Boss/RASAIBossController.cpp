// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Monster/Boss/RASAIBossController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/RASBlackBoardKey.h"

ARASAIBossController::ARASAIBossController()
{

}

void ARASAIBossController::RunAI()
{
	Super::RunAI();
}

void ARASAIBossController::StopAI()
{
	Super::StopAI();
}

void ARASAIBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void ARASAIBossController::SetupBlackBoard()
{

}
