// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/RASMapType.h"
#include "RASMapGenerateData.generated.h"

class ARASCorridor;
/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASMapGenerateData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	URASMapGenerateData();

	UPROPERTY(EditAnywhere, Category = Generate)
	float SeedNumber;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MaxRoomAmount;

	UPROPERTY(EditAnywhere, Category = Generate)
	float MaxDungeonTime;

	UPROPERTY(EditAnywhere, Category = Generate)
	TSubclassOf<class ARASRoom> StartRoomClass;

	UPROPERTY(EditAnywhere, Category = Generate)
	TMap<ERASRoomType, TSubclassOf<class ARASRoom>> RoomList;

	UPROPERTY(EditAnywhere, Category = Generate)
	TMap<ERASCorridorType, TSubclassOf<class ARASCorridor>> CorridorList;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MaxNpcRoomAmount;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MinNpcRoomAmount;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MinMainCorridorAmount;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MaxMainCorridorAmount;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MaxSpawnAttempts;

	UPROPERTY(EditAnywhere, Category = Generate)
	int32 MaxRestartFailures;
};
