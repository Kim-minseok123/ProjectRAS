// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_AttackTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_AttackTo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_AttackTo();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	int32 AttackNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	FName StartSocketName = "WeaponStart";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	FName EndSocketName = "WeaponEnd";

protected:
	virtual void MakeLineTrace(USkeletalMeshComponent* Attacker);

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
