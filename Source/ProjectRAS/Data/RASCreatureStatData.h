// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/RASCharacterStat.h"
#include "RASCreatureStatData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASCreatureStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URASCreatureStatData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TMap<FName, FRASCharacterStats> Stats;
};
