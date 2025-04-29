// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CircularRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_CircularRangeAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_CircularRangeAttack();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 AttackNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius;
};
