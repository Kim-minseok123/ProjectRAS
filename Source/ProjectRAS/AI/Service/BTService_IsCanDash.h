// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_IsCanDash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTService_IsCanDash : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_IsCanDash();

	UPROPERTY(EditAnywhere)
	float DashDistance = 300.f;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
