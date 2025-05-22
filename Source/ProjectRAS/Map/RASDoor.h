// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RASDoor.generated.h"

UCLASS()
class PROJECTRAS_API ARASDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARASDoor();

	void SetConnectedChunk(class ARASChunk* Chunk)
	{
		ConnectedChunk = Chunk;
	}

	class ARASChunk* GetConnectedChunk() const
	{
		return ConnectedChunk;
	}

	void SetOwnerChunk(class ARASChunk* Chunk);
	class ARASChunk* GetOwnerChunk() const;

	void CloseDoor();
	void OpenDoor();

	void SetupMoveable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<USceneComponent> DoorRootComponent;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorWall;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorLeft;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorRight;

	TObjectPtr<class ARASChunk> ConnectedChunk;

	TObjectPtr<class ARASChunk> OwnerChunk;
public:	
	virtual void Tick(float DeltaTime) override;

	const FRotator ClosedRot = FRotator(0.f, 0.f, 0.f);
	const FRotator OpenRot = FRotator(0.f, 90.f, 0.f);

	bool  bMoving = false;   // 열리거나 닫히는 중인가
	bool  bOpening = false;   // true: 열리는 중, false: 닫히는 중
	float Elapsed = 0.f;     // 경과 시간
	float Duration = 1.f;     // 애니메이션 길이(초)

	void StartMove(bool bOpen);

};
