// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PushBody.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_PushBody : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_PushBody();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
