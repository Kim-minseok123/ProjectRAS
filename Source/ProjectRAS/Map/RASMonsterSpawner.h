// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RASMonsterSpawner.generated.h"

UCLASS()
class PROJECTRAS_API ARASMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ARASMonsterSpawner();
	UFUNCTION(BlueprintCallable)
	void SpawnCreature();
	UFUNCTION()
	void CheckClearRoom();

	void SetRoom(class ARASRoom* InRoom) { OwnRoom = InRoom; }
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<TSubclassOf<class ARASMonster>> MonsterClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxMonsterCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MinMonsterCount;

	bool bIsSpawned;

	int SpawnCount;

	TObjectPtr<class ARASRoom> OwnRoom;
};
