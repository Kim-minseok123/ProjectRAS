// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_KnockBackTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_KnockBackTrigger : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_KnockBackTrigger();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
