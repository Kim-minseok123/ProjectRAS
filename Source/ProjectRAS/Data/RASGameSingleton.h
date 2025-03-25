// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/RASCharacterStat.h"
#include "Data/RASDamageData.h"
#include "RASGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRASGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	URASGameSingleton();
	static URASGameSingleton& Get();

public:

	FRASCharacterStats& GetStatForName(const FName InName);

	FCreatureDamageInfo& GetDamageInfoForName(const FName InName);

private:
	TMap<FName, FRASCharacterStats> CharacterStat;

	TMap<FName, FCreatureDamageInfo> DamageInfo;
};
