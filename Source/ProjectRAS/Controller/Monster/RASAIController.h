// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RASAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASAIController : public AAIController
{
	GENERATED_BODY()
public:
	ARASAIController();

	virtual void RunAI();
	virtual void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetupBlackBoard();

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
