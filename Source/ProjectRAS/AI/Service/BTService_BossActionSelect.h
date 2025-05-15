// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BossActionSelect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UBTService_BossActionSelect : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_BossActionSelect();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    int32 LastSkillIndex = -1;
	static constexpr float RepeatPenalty = 2.0f;
};
