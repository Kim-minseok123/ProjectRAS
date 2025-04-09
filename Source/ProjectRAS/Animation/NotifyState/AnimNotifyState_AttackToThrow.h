// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackToThrow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_AttackToThrow : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_AttackToThrow();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ARASProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackNum;

};
