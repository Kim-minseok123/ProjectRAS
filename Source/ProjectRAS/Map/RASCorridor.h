// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/RASChunk.h"
#include "Data/RASMapType.h"
#include "RASCorridor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASCorridor : public ARASChunk
{
	GENERATED_BODY()
public:
	ARASCorridor();
	
	virtual void BeginPlay() override;

	void SetDoorCollision();

	ERASCorridorType GetCorridorType() const { return CorridorType; }
protected:
	UPROPERTY(EditAnywhere, Category = "Corridor")
	ERASCorridorType CorridorType = ERASCorridorType::None;
};
