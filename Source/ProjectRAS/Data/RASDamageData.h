// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RASDamageData.generated.h"

USTRUCT(BlueprintType)
struct FCreatureDamageInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TMap<int, float> DamageInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaminaDamage")
	TMap<int, float> StaminaDamageInfo;

};

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASDamageData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URASDamageData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TMap<FName, FCreatureDamageInfo> Infos;
};
