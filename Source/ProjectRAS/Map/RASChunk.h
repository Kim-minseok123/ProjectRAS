// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/RASMapType.h"
#include "RASChunk.generated.h"

UCLASS()
class PROJECTRAS_API ARASChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	ARASChunk();

	ERASMapType GetMapType() const { return MapType; }
	virtual void SetupDoor();

	TArray<TObjectPtr<class ARASDoor>> GetDoors() const
	{
		return Doors;
	}
	TObjectPtr<class ARASDoor> GetStartDoor() const
	{
		return StartDoor;
	}

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ChunkRootComponent;

	UPROPERTY(EditAnywhere, Category = Type)
	ERASMapType MapType = ERASMapType::None;
	

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ExitsParent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> StartsParent;
	UPROPERTY(EditAnywhere, Category = Box)
	TObjectPtr<class UBoxComponent> CollisionBox;
	TArray<TObjectPtr<class ARASDoor>> Doors;

	TObjectPtr<class ARASDoor> StartDoor;
};
