// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/RASAIController.h"
#include "RASAIBossController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASAIBossController : public ARASAIController
{
	GENERATED_BODY()
public:
	ARASAIBossController();

	virtual void RunAI() override;
	virtual void StopAI() override;
	void SetTargetBlackboard();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupBlackBoard() override;
};
