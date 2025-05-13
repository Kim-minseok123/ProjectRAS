// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RASBossScoreData.generated.h"

USTRUCT(BlueprintType)
struct FSkillScoreData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Cooldown = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float IdealRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float BaseWeight = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DistanceWeight = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float HpWeight = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float StaminaWeight = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillDelay = 5.f;

	float LastUsedTime = -999.f;
};

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASBossScoreData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	URASBossScoreData();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillScoreData")
	TMap<int32, FSkillScoreData> SkillScoreDataMap;
};
