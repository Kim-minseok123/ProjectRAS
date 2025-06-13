// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayAudio.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_PlayAudio : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_PlayAudio();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


protected:
	UPROPERTY(EditAnywhere)
	FName AudioName;
};
