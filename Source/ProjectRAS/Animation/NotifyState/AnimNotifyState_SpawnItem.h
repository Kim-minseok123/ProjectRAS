// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SpawnItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotifyState_SpawnItem : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_SpawnItem();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Item")
	TSubclassOf<class AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Item")
	TObjectPtr<class AActor> ItemActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Item")
	FName SocketName;
};
