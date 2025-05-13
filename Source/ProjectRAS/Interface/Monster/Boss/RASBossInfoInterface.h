// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/RASBossScoreData.h"
#include "RASBossInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URASBossInfoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRAS_API IRASBossInfoInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FSkillScoreData& GetSkillScoreData(int32 InIdx) = 0;
	virtual int32 GetSkillScoreDataCount() = 0;
	virtual float GetHealthPercent() = 0;
	virtual float GetStaminaPercent() = 0;
};
