// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Monster/RASAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ARASAIController::ARASAIController()
{

}

void ARASAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		SetupBlackBoard();

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ARASAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ARASAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void ARASAIController::SetupBlackBoard()
{

}
