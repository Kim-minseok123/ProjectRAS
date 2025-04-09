// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetExecuteLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_SetExecuteLocation : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_SetExecuteLocation();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Offset = 200.f;
};
