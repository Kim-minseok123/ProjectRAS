// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RASSoundData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASSoundData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URASSoundData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, TSoftObjectPtr<class USoundCue>> BGMMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, TSoftObjectPtr<class USoundCue>> SFXMap;
};
