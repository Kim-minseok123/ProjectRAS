// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASRoom.h"
#include "Map/RASDoor.h"
#include "Components/ChildActorComponent.h"
#include "Map/RASMonsterSpawner.h"

ARASRoom::ARASRoom()
{
	MapType = ERASMapType::Room;
}

void ARASRoom::CloseDoors()
{
	for (auto& Door : Doors)
	{
		if (Door == nullptr)
		{
			continue;
		}
		Door->CloseDoor();
	}
	StartDoor->CloseDoor();
}

void ARASRoom::OpenDoors()
{
	for (auto& Door : Doors)
	{
		if (Door == nullptr)
		{
			continue;
		}
		Door->OpenDoor();
	}
	StartDoor->OpenDoor();
}

void ARASRoom::SetupMoveableDoor()
{
	for (auto& Door : Doors)
	{
		if (Door == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Door is null!, %s"), *(GetActorLocation().ToString()));
			continue;
		}
		Door->SetupMoveable();
	}
	if (StartDoor == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Door is null!, %s"), *(GetActorLocation().ToString()));
		return;
	}
	StartDoor->SetupMoveable();
}

void ARASRoom::BeginPlay()
{
	if (SpanwerClass)
	{
		const FVector ForwardCenter = GetActorLocation() +
			GetActorForwardVector() * 900.f + FVector(0.f, 0, 500.f);
		const FRotator SpawnRot = GetActorRotation();

		FActorSpawnParameters Params;
		Params.Owner = this;

		Spawner = GetWorld()->SpawnActor<ARASMonsterSpawner>(
			SpanwerClass, ForwardCenter, SpawnRot, Params);

		Spawner->SetRoom(this);
	}
}
