// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/RASMapType.h"
#include "RASMapGenerator.generated.h"


USTRUCT(BlueprintType)
struct FExitInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category="Generate")
    ERASMapType MapType;

    UPROPERTY(EditAnywhere, Category="Generate")
	TObjectPtr<class ARASDoor> Door;
};
class ARASCorridor;
class ARASRoom;
UCLASS()
class PROJECTRAS_API ARASMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ARASMapGenerator();

	bool GetDungeonComplete() { return bCompleteMapGenerate; }
protected:
	virtual void BeginPlay() override;

	void SetSeed();												// 시드 설정
	void SpawnMainRoom();										// 메인 룸 스폰
	void GenerateMap();											// 맵 루프 시작
	bool SpawnNextChunk();										// 다음 청크(룸 or 복도) 스폰
	bool SpawnAtExit(UWorld* World, const FExitInfo& ExitInfo);	// 스폰할 룸 or 복도 선택
	// 복도 생성
	bool SpawnCorridor(UWorld* World, const FExitInfo& ExitInfo, const FActorSpawnParameters& Params);
	// 룸 생성
	bool SpawnRoom(UWorld* World, const FExitInfo& ExitInfo, const FActorSpawnParameters& Params);
	// 충돌 검사 후 재시도 or 확정
	bool CheckForOverlap(class ARASChunk* InChunk);

	void RemoveNonConnectedChunks();							// 연결되지 않은 청크 삭제
	void FinishMapGenerate();									// 맵 생성 완료


    FRandomStream Stream;

	UPROPERTY(EditAnywhere, Category = Generate)
	TArray<FExitInfo> ExitsList;

	UPROPERTY(EditAnywhere, Category = Generate)
	TArray<TObjectPtr<class ARASChunk>> SpawnedChunks;

	UPROPERTY(EditAnywhere, Category = Generate)
	TObjectPtr<class URASMapGenerateData> MapGenerateData;

	UPROPERTY(EditAnywhere, Category = Generate)
	TArray<ERASRoomType> SpawnRoomTypeList;	// 스폰할 룸 타입 리스트

private:
	FTimerHandle GenerateMapTimerHandle;

	bool bCompleteMapGenerate = false;	// 맵 생성 완료 여부
};
