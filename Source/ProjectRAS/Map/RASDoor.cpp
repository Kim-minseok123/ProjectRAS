// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/RASDoor.h"
#include "Kismet/KismetMathLibrary.h"
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

void ARASDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bMoving) return;

    Elapsed += DeltaTime;
    float Alpha = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);

    // 보간
    const FRotator TargetRot = bOpening ? OpenRot : ClosedRot;
    const FRotator StartRot = bOpening ? ClosedRot : OpenRot;
    const FRotator NewRot = UKismetMathLibrary::RLerp(StartRot, TargetRot, Alpha, true);

    DoorLeft->SetRelativeRotation(NewRot);
    DoorRight->SetRelativeRotation(NewRot);

    if (Alpha >= 1.f)
    {
        bMoving = false;            
    }
}

void ARASDoor::OpenDoor()
{
    if (ConnectedChunk == nullptr) return;
    StartMove(true);
}

void ARASDoor::CloseDoor()
{
    if (ConnectedChunk == nullptr) return;
    StartMove(false);
}

void ARASDoor::StartMove(bool bOpen)
{
    if (bMoving && bOpening == bOpen) return; 

    bOpening = bOpen;
    bMoving = true;
    Elapsed = 0.f;
}

void ARASDoor::SetupMoveable()
{
	if (ConnectedChunk == nullptr)
	{
		DoorLeft->SetRelativeRotation(ClosedRot);
        DoorRight->SetRelativeRotation(ClosedRot);
	}
}

