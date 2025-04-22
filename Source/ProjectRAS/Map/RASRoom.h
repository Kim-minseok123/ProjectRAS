// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/RASChunk.h"
#include "RASRoom.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASRoom : public ARASChunk
{
	GENERATED_BODY()
public:
	ARASRoom();
	
	void SetupMoveableDoor();
protected:
	
};
