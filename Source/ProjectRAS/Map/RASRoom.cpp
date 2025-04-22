// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASRoom.h"
#include "Map/RASDoor.h"
#include "Components/ChildActorComponent.h"
ARASRoom::ARASRoom()
{
	MapType = ERASMapType::Room;

	
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
