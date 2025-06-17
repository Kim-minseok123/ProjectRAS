// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASChunk.h"
#include "Map/RASDoor.h"
#include "Components/BoxComponent.h"
#include "Character/Player/RASPlayer.h"

ARASChunk::ARASChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	ChunkRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = ChunkRootComponent;

	ExitsParent = CreateDefaultSubobject<USceneComponent>(TEXT("ExitsParent"));
	ExitsParent->SetupAttachment(ChunkRootComponent);

	StartsParent = CreateDefaultSubobject<USceneComponent>(TEXT("StartsParent"));
	StartsParent->SetupAttachment(ChunkRootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
}

void ARASChunk::SetupDoor()
{
	TArray<USceneComponent*> ChildComponents = ExitsParent->GetAttachChildren();
	for (USceneComponent* Child : ChildComponents)
	{
		if (Child->IsA<UChildActorComponent>())
		{
			UChildActorComponent* ChildActor = Cast<UChildActorComponent>(Child);
			if (ChildActor)
			{
				ARASDoor* Door = Cast<ARASDoor>(ChildActor->GetChildActor());
				if (Door)
				{
					Doors.Add(Door);
					Door->SetOwnerChunk(this);
				}
			}
		}
	}

	TArray<USceneComponent*> StartComponents = StartsParent->GetAttachChildren();
	if(StartComponents.Num() != 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chunk %s has more than one start door!"), *GetName());
		return;
	}
	USceneComponent* Start = StartComponents[0];
	
	if (Start->IsA<UChildActorComponent>())
	{
		UChildActorComponent* ChildActor = Cast<UChildActorComponent>(Start);
		if (ChildActor)
		{
			ARASDoor* Door = Cast<ARASDoor>(ChildActor->GetChildActor());
			if (Door)
			{
				StartDoor = Door;
				Door->SetOwnerChunk(this);
			}
		}
	}
	
}

void ARASChunk::PlayerInChunk(ARASPlayer* Player)
{
	if (Player == nullptr) return;
	Player->SetCurrentChunk(this);
	bArrive = true;
}

void ARASChunk::PlayerOutChunk(ARASPlayer* Player)
{
	//Player->SetCurrentChunk(nullptr);
}

void ARASChunk::BeginPlay()
{
	Super::BeginPlay();
	
}



