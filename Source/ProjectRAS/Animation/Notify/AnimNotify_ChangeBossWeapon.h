// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ChangeBossWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UAnimNotify_ChangeBossWeapon : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_ChangeBossWeapon();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponChange")
	uint8 bWeaponOn : 1;
};
