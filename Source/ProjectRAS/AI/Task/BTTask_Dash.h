// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Dash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTTask_Dash : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Dash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash Settings")
	float DashSpeed = 200.f;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
