// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MoveToLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_MoveToLocation : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_MoveToLocation();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveToLocation")
	float DistanceValue;

    struct FRuntimeData
    {
        FVector  StartPos;
        float     Travelled = 0.f;
        float     Elapsed = 0.f;
        float     Duration = 0.f;
        TWeakObjectPtr<AActor> Target;
    };
    
    TMap<TWeakObjectPtr<USkeletalMeshComponent>, FRuntimeData> Runtime;
};
