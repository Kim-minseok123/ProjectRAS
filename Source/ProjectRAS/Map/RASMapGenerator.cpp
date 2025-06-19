// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/RASMapGenerator.h"
#include "Data/RASMapGenerateData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/RASUtils.h"
#include "Map/RASChunk.h"
#include "Map/RASDoor.h"
#include "Map/RASRoom.h"
#include "Map/RASCorridor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASUIComponent.h"

ARASMapGenerator::ARASMapGenerator()
{

	static ConstructorHelpers::FObjectFinder<URASMapGenerateData>
		MapGenerateDataAsset(TEXT("/Script/ProjectRAS.RASMapGenerateData'/Game/1_ProjectRAS/Data/DA_RASMapGeneratorData.DA_RASMapGeneratorData'"));
	if (MapGenerateDataAsset.Object)
	{
		MapGenerateData = MapGenerateDataAsset.Object;
	}

	Stream.Initialize(MapGenerateData->SeedNumber);
}
// Called when the game starts or when spawned
void ARASMapGenerator::BeginPlay()
{
	Super::BeginPlay();

	SetSeed();

	SpawnMainRoom();

	GenerateMap();
}

void ARASMapGenerator::SetSeed()
{
	if (MapGenerateData && MapGenerateData->SeedNumber >= 0)
		Stream.Initialize(MapGenerateData->SeedNumber);
	else
		UKismetMathLibrary::SeedRandomStream(Stream);
}

void ARASMapGenerator::SpawnMainRoom()
{
	if (!MapGenerateData || !MapGenerateData->StartRoomClass)
		return;

	// 스폰 할 방 타입 리스트 초기화
	SpawnRoomTypeList.Empty();
	int32 NPCRoomCount = Stream.RandRange(MapGenerateData->MinNpcRoomAmount, MapGenerateData->MaxNpcRoomAmount);
	for (int32 i = 0; i < NPCRoomCount; i++)
	{
		SpawnRoomTypeList.Add(ERASRoomType::NPC);
	}

	SpawnRoomTypeList.Add(ERASRoomType::Boss);

	for (int32 i = SpawnRoomTypeList.Num(); i < MapGenerateData->MaxRoomAmount; i++)
	{
		SpawnRoomTypeList.Add(ERASRoomType::Normal);
	}

	RASUtils::ShuffleTArray(SpawnRoomTypeList, Stream);

	UWorld* World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARASChunk* Main = Cast<ARASChunk>(World->SpawnActor<AActor>(MapGenerateData->StartRoomClass,
		GetActorLocation(), GetActorRotation(), Params));
	if (!Main)
		return;

	Main->SetupDoor();

	TArray<TObjectPtr<ARASDoor>> Doors = Main->GetDoors();
	RASUtils::ShuffleTArray(Doors, Stream);

	int32 CorridorCount = Stream.RandRange(MapGenerateData->MinMainCorridorAmount, MapGenerateData->MaxMainCorridorAmount);

	for (int32 i = 0; i < CorridorCount; ++i)
	{
		ExitsList.Add({ Main->GetMapType(), Doors[i] });
	}

	SpawnedChunks.Add(Main);

	UE_LOG(LogTemp, Log, TEXT("메인 탈출구 %d개"), ExitsList.Num());
}

void ARASMapGenerator::GenerateMap()
{
	int32 ConsecutiveFailures = 0;

	const int32 MaxFailuresBeforeRestart = MapGenerateData->MaxRestartFailures;

	while (SpawnRoomTypeList.Num() > 0 && ExitsList.Num() > 0)
	{
		if (!SpawnNextChunk())
		{
			++ConsecutiveFailures;
			UE_LOG(LogTemp, Warning, TEXT("GenerateMap: SpawnNextChunk failures %d/%d"), ConsecutiveFailures, MaxFailuresBeforeRestart);

			if (ConsecutiveFailures >= MaxFailuresBeforeRestart)
			{
				UE_LOG(LogTemp, Warning, TEXT("GenerateMap: Too many consecutive failures, restarting generation"));

				for (ARASChunk* Chunk : SpawnedChunks)
				{
					if (Chunk)
					{
						ARASRoom* Room = Cast<ARASRoom>(Chunk);
						if (Room)
						{
							Room->DestroySpawner();
						}
						Chunk->Destroy();
					}
				}
				SpawnedChunks.Empty();
				ExitsList.Empty();
				SpawnRoomTypeList.Empty();

				SpawnMainRoom();

				ConsecutiveFailures = 0;
				continue;
			}

			RASUtils::ShuffleTArray(SpawnRoomTypeList, Stream);
			continue;
		}

		ConsecutiveFailures = 0;
	}

	bCompleteMapGenerate = true;

	RemoveNonConnectedChunks();
	FinishMapGenerate();

}

bool ARASMapGenerator::SpawnNextChunk()
{
	UWorld* World = GetWorld();
	if (!World || SpawnRoomTypeList.Num() == 0 || ExitsList.Num() == 0)
		return false;

	UE_LOG(LogTemp, Log, TEXT(" 스폰 시작"));

	for (int32 Attempt = 0; Attempt < MapGenerateData->MaxSpawnAttempts; ++Attempt)
	{
		int32 ExitIndex = Stream.RandRange(0, ExitsList.Num() - 1);
		FExitInfo ExitInfo = ExitsList[ExitIndex];

		if (SpawnAtExit(World, ExitInfo))
		{
			ExitsList.RemoveAt(ExitIndex);
			UE_LOG(LogTemp, Log, TEXT("스폰된 후 남은 탈출구 %d개"), ExitsList.Num());

			return true;
		}

		RASUtils::ShuffleTArray(SpawnRoomTypeList, Stream);

		if (ExitsList.Num() == 0)
			break;
	}
	return false;
}

bool ARASMapGenerator::SpawnAtExit(UWorld* World, const FExitInfo& ExitInfo)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (ExitInfo.MapType)
	{
	case ERASMapType::Room:
		UE_LOG(LogTemp, Log, TEXT(" 룸 타입 선택됨 , Vector : %s"), *ExitInfo.Door->GetActorLocation().ToString());
		return SpawnCorridor(World, ExitInfo, Params);
		break;
	case ERASMapType::Corridor:
		UE_LOG(LogTemp, Log, TEXT(" 복도 타입 선택됨, Vector : %s"), *ExitInfo.Door->GetActorLocation().ToString());
		if (Stream.RandRange(0, 2) < 2)
		{
			UE_LOG(LogTemp, Log, TEXT("룸 선택"));
			if (SpawnRoom(World, ExitInfo, Params))
			{
				return true;
			}
			UE_LOG(LogTemp, Warning, TEXT("SpawnRoom 실패, 복도로 폴백"));
			return SpawnCorridor(World, ExitInfo, Params);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("복도 선택"));
			return SpawnCorridor(World, ExitInfo, Params);
		}

	default:
		UE_LOG(LogTemp, Warning, TEXT("SpawnAtExit: invalid MapType"));
		return false;
	}
}

bool ARASMapGenerator::SpawnCorridor(UWorld* World, const FExitInfo& ExitInfo, const FActorSpawnParameters& Params)
{
	TArray<int32> CorridorTypes = { 0, 1, 2 };
	RASUtils::ShuffleTArray(CorridorTypes, Stream);

	for (int32 CorridorType : CorridorTypes)
	{
		TSubclassOf<AActor> CorridorClass;
		switch (CorridorType)
		{
		case 0:
			CorridorClass = MapGenerateData->CorridorList[ERASCorridorType::Straight];
			break;
		case 1:
			CorridorClass = MapGenerateData->CorridorList[ERASCorridorType::BentRight];
			break;
		case 2:
			CorridorClass = MapGenerateData->CorridorList[ERASCorridorType::BentLeft];
			break;
		default:
			continue;
		}

		FVector SpawnLocation = ExitInfo.Door->GetActorLocation();
		FRotator SpawnRotation = ExitInfo.Door->GetActorRotation();

		ARASChunk* Chunk = Cast<ARASChunk>(
			World->SpawnActor<AActor>(CorridorClass, SpawnLocation, SpawnRotation, Params));
		if (!Chunk) return false;

		Chunk->SetupDoor();                      
		Chunk->CollisionBox->UpdateBounds();      
		
		if (!CheckForOverlap(Chunk))              
		{
			Chunk->Destroy();
			return false;
		}
		Chunk->GetStartDoor()->SetConnectedChunk(ExitInfo.Door->GetOwnerChunk());
		ExitInfo.Door->SetConnectedChunk(Chunk);

		UE_LOG(LogTemp, Log, TEXT("복도 타입 %d 스폰 성공"), CorridorType);

		for (ARASDoor* Door : Chunk->GetDoors())
			ExitsList.Add({ Chunk->GetMapType(), Door });

		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnCorridor: 모든 복도 타입 실패"));
	return false;
}

bool ARASMapGenerator::SpawnRoom(UWorld* World, const FExitInfo& ExitInfo, const FActorSpawnParameters& Params)
{
	if (SpawnRoomTypeList.Num() == 0)
		return false;

	ERASRoomType Type = SpawnRoomTypeList[0];
	if (Type == ERASRoomType::None)
		return false;
	UE_LOG(LogTemp, Log, TEXT("%d 룸 스폰 시도"), Type);

	FVector SpawnLocation = ExitInfo.Door->GetActorLocation();
	FRotator SpawnRotation = ExitInfo.Door->GetActorRotation();

	ARASChunk* Chunk = Cast<ARASChunk>(
		World->SpawnActor<AActor>(MapGenerateData->RoomList[Type], SpawnLocation, SpawnRotation, Params));
	if (!Chunk) return false;

	Chunk->SetupDoor();                      
	Chunk->CollisionBox->UpdateBounds();     

	if (!CheckForOverlap(Chunk))             
	{
		Chunk->Destroy();
		return false;
	}
	
	Chunk->GetStartDoor()->SetConnectedChunk(ExitInfo.Door->GetOwnerChunk());

	ExitInfo.Door->SetConnectedChunk(Chunk);

	UE_LOG(LogTemp, Log, TEXT("%d 룸 스폰 완료"), Type);

	SpawnRoomTypeList.RemoveAt(0);

	TArray<TObjectPtr<ARASDoor>> Doors = Chunk->GetDoors();
	RASUtils::ShuffleTArray(Doors, Stream);

	int32 CorridorCount = Stream.RandRange(2, 3);
	CorridorCount = FMath::Min(CorridorCount, Doors.Num());

	for (int32 i = 0; i < CorridorCount; ++i)
	{
		ExitsList.Add({ Chunk->GetMapType(), Doors[i] });
	}
	return true;
}

bool ARASMapGenerator::CheckForOverlap(class ARASChunk* InChunk)
{
	if (!InChunk)
		return false;

	FBox NewBox = InChunk->CollisionBox->Bounds.GetBox();
	for (ARASChunk* Existing : SpawnedChunks)
	{
		if (Existing)
		{
			FBox ExistingBox = Existing->CollisionBox->Bounds.GetBox();
			if (NewBox.Intersect(ExistingBox))
			{
				UE_LOG(LogTemp, Warning, TEXT("CheckForOverlap: Overlapping with existing chunk"));
				return false;
			}
		}
	}

	SpawnedChunks.Add(InChunk);
	return true;
}
// 시간 복잡도 O(N+E)
void ARASMapGenerator::RemoveNonConnectedChunks()
{
	// Corridor 청크만 수집해서 인접 리스트 초기화
	TMap<ARASChunk*, TArray<ARASChunk*>> CorrAdj;
	TSet<ARASChunk*> CorridorSet;
	for (ARASChunk* Chunk : SpawnedChunks)
	{
		if (Chunk && Chunk->GetMapType() == ERASMapType::Corridor)
		{
			CorridorSet.Add(Chunk);
			CorrAdj.FindOrAdd(Chunk);
		}
	}

	// Corridor↔Corridor 간 인접 정보 구축
	for (ARASChunk* C : CorridorSet)
	{
		TArray<ARASDoor*> AllDoors = C->GetDoors();
		if (C->GetStartDoor())
			AllDoors.Add(C->GetStartDoor());

		for (ARASDoor* D : AllDoors)
		{
			if (!D) continue;
			ARASChunk* N = D->GetConnectedChunk();
			if (N && N->GetMapType() == ERASMapType::Corridor)
			{
				CorrAdj[C].AddUnique(N);
			}
		}
	}

	// 각 Corridor 연결 을 BFS로 순회
	TSet<ARASChunk*> Visited;
	TSet<ARASChunk*> ToRemove;
	TQueue<ARASChunk*> Q;

	for (ARASChunk* Start : CorridorSet)
	{
		if (Visited.Contains(Start))
			continue;

		TArray<ARASChunk*> Component;
		TSet<ARASChunk*> AdjacentRooms;

		Visited.Add(Start);
		Q.Enqueue(Start);

		while (!Q.IsEmpty())
		{
			ARASChunk* Cur;
			Q.Dequeue(Cur);
			Component.Add(Cur);

			TArray<ARASDoor*> AllDoors = Cur->GetDoors();
			if (Cur->GetStartDoor())
				AllDoors.Add(Cur->GetStartDoor());

			for (ARASDoor* D : AllDoors)
			{
				if (!D) continue;
				ARASChunk* N = D->GetConnectedChunk();
				if (N && N->GetMapType() == ERASMapType::Room)
				{
					AdjacentRooms.Add(N);
				}
			}

			for (ARASChunk* Neigh : CorrAdj[Cur])
			{
				if (!Visited.Contains(Neigh))
				{
					Visited.Add(Neigh);
					Q.Enqueue(Neigh);
				}
			}
		}

		// 룸에 인접한 개수가 2개 미만이면, 이 성분의 모든 corridor를 제거 대상
		if (AdjacentRooms.Num() < 2)
		{
			for (ARASChunk* C : Component)
				ToRemove.Add(C);
		}
	}

	// 제거  Corridor들만 Disconnect → SpawnedChunks 제거 → Destroy
	auto DisconnectChunk = [&](ARASChunk* ChunkToRemove)
		{
			auto DisconnectDoor = [&](ARASDoor* Door)
				{
					if (!Door) return;
					ARASChunk* Neigh = Door->GetConnectedChunk();
					if (!Neigh) return;

					if (Neigh->GetStartDoor() &&
						Neigh->GetStartDoor()->GetConnectedChunk() == ChunkToRemove)
					{
						Neigh->GetStartDoor()->SetConnectedChunk(nullptr);
					}
					for (ARASDoor* ND : Neigh->GetDoors())
					{
						if (ND && ND->GetConnectedChunk() == ChunkToRemove)
							ND->SetConnectedChunk(nullptr);
					}
				};

			DisconnectDoor(ChunkToRemove->GetStartDoor());
			for (ARASDoor* D : ChunkToRemove->GetDoors())
				DisconnectDoor(D);
		};

	for (ARASChunk* Chunk : ToRemove)
	{
		DisconnectChunk(Chunk);
		SpawnedChunks.RemoveSingleSwap(Chunk);
		UE_LOG(LogTemp, Log, TEXT("RemoveNonConnectedChunks: Removed %s"), *Chunk->GetName());
		Chunk->Destroy();
	}
}
// 이전 코드 시간 복잡도 N^2
//void ARASMapGenerator::RemoveNonConnectedChunks()
//{
//	bool bRemovedAny = true;
//
//	while (bRemovedAny)
//	{
//		bRemovedAny = false;
//		TArray<ARASChunk*> ToRemove;
//
//		for (ARASChunk* Chunk : SpawnedChunks)
//		{
//			if (!Chunk || Chunk->GetMapType() != ERASMapType::Corridor)
//				continue;
//
//			int32 ConnectionCount = 0;
//			if (Chunk->GetStartDoor() && Chunk->GetStartDoor()->GetConnectedChunk())
//				ConnectionCount++;
//			for (ARASDoor* Door : Chunk->GetDoors())
//			{
//				if (Door && Door->GetConnectedChunk())
//					ConnectionCount++;
//			}
//
//			if (ConnectionCount <= 1)
//			{
//				ToRemove.Add(Chunk);
//			}
//		}
//
//		if (ToRemove.Num() > 0)
//		{
//			bRemovedAny = true;
//
//			for (ARASChunk* Chunk : ToRemove)
//			{
//				auto DisconnectNeighbor = [&](ARASDoor* Door)
//					{
//						if (!Door) return;
//						ARASChunk* Neighbor = Door->GetConnectedChunk();
//						if (!Neighbor) return;
//
//						if (Neighbor->GetStartDoor() &&
//							Neighbor->GetStartDoor()->GetConnectedChunk() == Chunk)
//						{
//							Neighbor->GetStartDoor()->SetConnectedChunk(nullptr);
//						}
//
//						for (ARASDoor* ND : Neighbor->GetDoors())
//						{
//							if (ND->GetConnectedChunk() == Chunk)
//								ND->SetConnectedChunk(nullptr);
//						}
//					};
//
//				DisconnectNeighbor(Chunk->GetStartDoor());
//				for (ARASDoor* Door : Chunk->GetDoors())
//					DisconnectNeighbor(Door);
//
//				SpawnedChunks.RemoveSingleSwap(Chunk);
//
//				UE_LOG(LogTemp, Log, TEXT("RemoveNonConnectedChunks: Removed %s"), *Chunk->GetName());
//
//				Chunk->Destroy();
//			}
//		}
//	}
//}
void ARASMapGenerator::FinishMapGenerate()
{
	FBox2D MapBounds;
	for (ARASChunk* Chunk : SpawnedChunks)
	{
		const FBox B3 = Chunk->CollisionBox->Bounds.GetBox();
		MapBounds+= FBox2D(FVector2D(B3.Min), FVector2D(B3.Max));

		ARASRoom* Room = Cast<ARASRoom>(Chunk);
		if (Room)
		{
			Room->SetSpawner();
			Room->SetupMoveableDoor();
		}

		ARASCorridor* Corridor = Cast<ARASCorridor>(Chunk);
		if (Corridor)
		{
			Corridor->SetDoorCollision();
		}
	}
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	Cast<ARASPlayer>(PC->GetPawn())->GetUIComponent()->SetMapUI(SpawnedChunks);
}
