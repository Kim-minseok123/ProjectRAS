// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASCorridor.h"
#include "Map/RASDoor.h"

ARASCorridor::ARASCorridor()
{
	MapType = ERASMapType::Corridor;
}

void ARASCorridor::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARASCorridor::SetDoorCollision()
{
	for (auto& Door : Doors)
	{
		if (Door == nullptr)
		{
			continue;
		}
		Door->SetNoCollision();
	}
	if (StartDoor != nullptr) 
	{
		StartDoor->SetNoCollision();
	}
}
