// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/RASAIController.h"
#include "RASAICommonController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASAICommonController : public ARASAIController
{
	GENERATED_BODY()
public:
	ARASAICommonController();

	virtual void RunAI() override;
	virtual void StopAI() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupBlackBoard() override;
};
