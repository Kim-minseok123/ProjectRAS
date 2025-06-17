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

	void CloseDoors();
	void OpenDoors();

	void SetSpawner();
	void DestroySpawner();

	void SetupMoveableDoor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Spawn")
	TSubclassOf<class ARASMonsterSpawner> SpanwerClass;  

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Spawn")
	TObjectPtr<class ARASMonsterSpawner> Spawner;

	// 스폰이 됐던 룸인지 확인
	UPROPERTY(EditAnywhere)
	uint8 bMonsterSpawn : 1;

};
