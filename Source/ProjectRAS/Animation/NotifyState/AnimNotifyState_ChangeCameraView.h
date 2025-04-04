// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ChangeCameraView.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_ChangeCameraView : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_ChangeCameraView();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
