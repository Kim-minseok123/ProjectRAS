// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASMonsterSpawner.h"
#include "Character/Monster/RASMonster.h"
#include "Map/RASRoom.h"

// Sets default values
ARASMonsterSpawner::ARASMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxMonsterCount = 3;
	MinMonsterCount = 2;
	SpawnCount = 0;
	bIsSpawned = false;
}

void ARASMonsterSpawner::SpawnCreature()
{
	if (bIsSpawned || bIsMonsterSpawn == false)
	{
		return;
	}
	if(OwnRoom == nullptr)
	{
		return;
	}
	OwnRoom->CloseDoors();
	bIsSpawned = true;

	SpawnCount = FMath::RandRange(MinMonsterCount, MaxMonsterCount);


	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	const FVector Center = GetActorLocation();

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		const int32 Idx = FMath::RandRange(0, MonsterClasses.Num() - 1);
		TSubclassOf<ARASMonster> MonsterClass = MonsterClasses[Idx];
		if (!MonsterClass) { continue; }

		const float OffX = FMath::RandRange(-600.f, 600.f);
		const float OffY = FMath::RandRange(-600.f, 600.f);
		FVector Pos = Center + FVector(OffX, OffY, 0.f);

		FHitResult Hit;
		const FVector Start = Pos + FVector(0.f, 0.f, 200.f);
		const FVector End = Pos - FVector(0.f, 0.f, 200.f);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic))
		{
			Pos.Z = Hit.Location.Z;
		}

		const float RandomYaw = FMath::RandRange(-180.f, 180.f);
		const FRotator Rot(0.f, RandomYaw, 0.f);

		ARASMonster* Monster = GetWorld()->SpawnActor<ARASMonster>(MonsterClass, Pos, Rot, SpawnParams);

		Monster->OnRoomClear.BindUObject(this, &ARASMonsterSpawner::CheckClearRoom);
	}
	bIsSpawned = true;
}

void ARASMonsterSpawner::CheckClearRoom()
{
	SpawnCount--;
	if (SpawnCount > 0)
	{
		return;
	}
	if (OwnRoom == nullptr)
	{
		return;
	}
	OwnRoom->OpenDoors();
}


void ARASMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARASMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

