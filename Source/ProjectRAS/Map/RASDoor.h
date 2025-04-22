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
	TObjectPtr<USceneComponent> DoorRootComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorWall = nullptr;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorLeft = nullptr;
	UPROPERTY(EditAnywhere, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorRight = nullptr;

	TObjectPtr<class ARASChunk> ConnectedChunk = nullptr;

	TObjectPtr<class ARASChunk> OwnerChunk = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
