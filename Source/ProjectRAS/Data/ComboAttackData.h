// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API UComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UComboAttackData();

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;
	
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TMap<FName, float> EffectiveFrameCount;
};
