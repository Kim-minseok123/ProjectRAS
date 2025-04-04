// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DeathEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_DeathEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_DeathEvent();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	void ChangeDeathCollision(USkeletalMeshComponent* MeshComp);
};
