// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_BossAttackTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_BossAttackTo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_BossAttackTo();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	int32 AttackNum;

protected:
	virtual void MakeSweepTrace(class ARASBossMonster* Boss);

	UPROPERTY(EditAnywhere, Category="AttackTrace")
	float TraceRadius = 8.f;
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
