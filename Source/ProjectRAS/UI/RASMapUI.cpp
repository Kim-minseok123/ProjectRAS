// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMapUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Map/RASChunk.h"
#include "Map/RASDoor.h"
#include "Containers/Queue.h"
#include "UI/RASMapButton.h"
#include "Components/BoxComponent.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASUIComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Data/RASMapType.h"


URASMapUI::URASMapUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
    

}

void URASMapUI::BuildMapUI(const TArray<TObjectPtr<ARASChunk>>& Spawned, ARASChunk* StartChunk, class ARASPlayer* InPlayer)
{
	if (!MapCanvas || !MapButtonClass || !IsValid(StartChunk))
		return;
	if (InPlayer == nullptr) return;

	Player = InPlayer;
	ExitButton->OnClicked.AddDynamic(this, &URASMapUI::ExitButtonClick);

	MapCanvas->ClearChildren();

	/* 0. 월드 → 셀 좌표 변환 함수 ------------------------------------------------*/
	const FVector  WorldOrigin = StartChunk->GetActorLocation();
	auto WorldToCell = [&](const FVector& Anchor) -> FIntPoint
		{
			const FVector D = Anchor - WorldOrigin;
			return FIntPoint(
				FMath::RoundToInt(D.X / RAS_CellWorldUnit),
				FMath::RoundToInt(D.Y / RAS_CellWorldUnit));
		};

	/* 1. BFS 로 모든 청크 조사 ---------------------------------------------------*/
	struct FChunkInfo
	{
		ARASChunk* Chunk = nullptr;
		FIntPoint  Cell;   // 셀 크기
		FIntPoint  GPos;   // 원점 기준 셀 위치
		float      YawSnap = 0.f;
	};
	TArray<FChunkInfo> ChunkInfos;

	TQueue<ARASChunk*> Queue;
	TSet  <ARASChunk*> Visited;
	Visited.Add(StartChunk);
	Queue.Enqueue(StartChunk);

	FIntPoint MinXY(INT_MAX, INT_MAX);
	FIntPoint MaxXY(INT_MIN, INT_MIN);

	auto PushInfo = [&](ARASChunk* C)
		{
			const FIntPoint Cell = GetCellSize(C);
			const FIntPoint GPos = WorldToCell(C->CollisionBox->Bounds.Origin);
			const float     Yaw = 90.f * FMath::RoundToInt(C->GetActorRotation().Yaw / 90.f);
			ChunkInfos.Add({ C, Cell, GPos, Yaw });

			MinXY.X = FMath::Min(MinXY.X, GPos.X - Cell.X);
			MinXY.Y = FMath::Min(MinXY.Y, GPos.Y - Cell.Y);
			MaxXY.X = FMath::Max(MaxXY.X, GPos.X + Cell.X);
			MaxXY.Y = FMath::Max(MaxXY.Y, GPos.Y + Cell.Y);
		};

	while (!Queue.IsEmpty())
	{
		ARASChunk* Cur = nullptr;
		Queue.Dequeue(Cur);
		if (!IsValid(Cur))
			continue;

		PushInfo(Cur);

		for (ARASDoor* D : Cur->GetDoors())
		{
			if (!IsValid(D))             continue;
			ARASChunk* N = D->GetConnectedChunk();
			if (!IsValid(N) || Visited.Contains(N)) continue;
			Visited.Add(N);
			Queue.Enqueue(N);
		}
	}

	/* 2. 좌표 보정량 (Offset) 계산 ----------------------------------------------*/
	const FIntPoint Offset(-MinXY.X, -MinXY.Y);   // 셀 단위

	/* 3. 위젯 생성 및 배치 -------------------------------------------------------*/
	for (const FChunkInfo& Info : ChunkInfos)
	{
		if (!IsValid(Info.Chunk))
			continue;

		URASMapButton* Tile = CreateWidget<URASMapButton>(GetWorld(), MapButtonClass);
		Tile->Init(Info.Chunk, Player);
		MapButtons.Add(Tile);
		if (Info.Chunk->GetMapType() == ERASMapType::Corridor)
			UE_LOG(LogTemp, Log, TEXT("%f"), Info.YawSnap);
		const FVector2D SizePx(
			Info.Cell.X * RAS_CellPixel,
			Info.Cell.Y * RAS_CellPixel);

		const FVector2D PosPx(
			(Info.GPos.X + Offset.X) * RAS_CellPixel / 2,
			(Info.GPos.Y + Offset.Y) * RAS_CellPixel / 2);

		if (UCanvasPanelSlot* S = Cast<UCanvasPanelSlot>(MapCanvas->AddChild(Tile)))
		{
			S->SetAutoSize(false);
			S->SetSize(SizePx);
			S->SetAlignment(FVector2D(0.5f, 0.5f));
			S->SetPosition(PosPx);
			Tile->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			Tile->SetRenderTransformAngle(-Info.YawSnap);
		}
	}
	
}

void URASMapUI::FoundMapShow()
{
	for (URASMapButton* Button : MapButtons)
	{
		if (Button)
		{
			if (Button->CheckVisitChunk())
			{
				Button->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				//Button->SetVisibility(ESlateVisibility::Hidden);
			}
			Button->SetCurrentChunk();
		}
	}
}

void URASMapUI::ExitButtonClick()
{
	Player->GetUIComponent()->HideMapUI();
}


FIntPoint URASMapUI::GetCellSize(const class ARASChunk* Chunk) const
{
    const FVector2D SizeCm = Chunk->ChunkSize;
    const int32  W = FMath::Max(1, FMath::RoundToInt(SizeCm.X / RAS_CellWorldUnit));
    const int32  H = FMath::Max(1, FMath::RoundToInt(SizeCm.Y / RAS_CellWorldUnit));
    return FIntPoint(W, H);
}

