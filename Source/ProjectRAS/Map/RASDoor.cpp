// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASDoor.h"

// Sets default values
ARASDoor::ARASDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRootComponent"));
	RootComponent = DoorRootComponent;

	DoorWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWall"));
	DoorWall->SetupAttachment(DoorRootComponent);

	DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	DoorLeft->SetupAttachment(DoorWall);
	DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));
	DoorRight->SetupAttachment(DoorWall);

}

void ARASDoor::SetOwnerChunk(class ARASChunk* Chunk)
{
	OwnerChunk = Chunk;
}

class ARASChunk* ARASDoor::GetOwnerChunk() const
{
	return OwnerChunk;
}

// Called when the game starts or when spawned
void ARASDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARASDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ARASDoor::CloseDoor()
{
	DoorLeft->SetRelativeRotation(FRotator(0,0,0));
	DoorRight->SetRelativeRotation(FRotator(0,0,0));
}

void ARASDoor::OpenDoor()
{
	DoorLeft->SetRelativeRotation(FRotator(0, 90, 0));
	DoorRight->SetRelativeRotation(FRotator(0, 90, 0));
}

void ARASDoor::SetupMoveable()
{
	if (ConnectedChunk != nullptr)
	{
		// Open Door 게임 플레이 델리게이트에 설정하기

		// Close Door 게임 플레이 델리게이트에 설정하기
	}
	else
	{
		CloseDoor();
	}
}

