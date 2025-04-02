// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MoveLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_MoveLocation : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_MoveLocation();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Move)
	float MoveValue;
};
