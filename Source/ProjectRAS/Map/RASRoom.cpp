// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASRoom.h"
#include "Map/RASDoor.h"
#include "Components/ChildActorComponent.h"
#include "Map/RASMonsterSpawner.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASCombatComponent.h"

ARASRoom::ARASRoom()
{
	MapType = ERASMapType::Room;
	bMonsterSpawn = true;
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
	if (StartDoor != nullptr) 
	{
		StartDoor->CloseDoor();
	}
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
	if (StartDoor != nullptr) 
	{
		StartDoor->OpenDoor();
	}
}

void ARASRoom::SetSpawner()
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

		Spawner->SetRoom(this, bMonsterSpawn);
	}
}

void ARASRoom::DestroySpawner()
{
	if (Spawner)
	{
		Spawner->Destroy();
		Spawner = nullptr;
	}
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

void ARASRoom::PlayerToBoss()
{
	Cast<ARASPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCombatComponent()->PressTab();
	CloseDoors();
}

void ARASRoom::BeginPlay()
{
	Super::BeginPlay();
}
