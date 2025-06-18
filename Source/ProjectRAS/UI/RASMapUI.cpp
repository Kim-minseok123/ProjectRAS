// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMapUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Map/RASChunk.h"
#include "Map/RASDoor.h"
#include "Containers/Queue.h"
#include "UI/RASMapButton.h"
#include "Components/BoxComponent.h"

URASMapUI::URASMapUI(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
    MapSize = FVector2D(3840, 2160);

}

void URASMapUI::InitMap(FBox2D InBox)
{
    Bounds = InBox;
    const FVector2D MapUU = Bounds.GetSize();

    float Scale = FMath::Min(
        MapSize.X / MapUU.X,
        MapSize.Y / MapUU.Y
    );
    PixelPerUU = Scale;

    const FVector2D CanvasSz = MapUU * PixelPerUU;
    if (auto Root = Cast<UCanvasPanelSlot>(MapCanvas->Slot))
    {
        Root->SetSize(CanvasSz);
    }

}

void URASMapUI::BuildMapUI(const TArray<TObjectPtr<class ARASChunk>>& Spawned)
{
    if (!MapCanvas || !MapButtonClass)
        return;
    if (Spawned.Num() == 0) return;

    MapCanvas->ClearChildren();
    TSet<ARASChunk*> Visited;
    TQueue<ARASChunk*> Q;
    
    Q.Enqueue(Spawned[0]);
    Visited.Add(Spawned[0]);

    while (Q.IsEmpty() == false)
    {
        ARASChunk* Cur; Q.Dequeue(Cur);
        if (!Cur) continue;
        // ① 타일 픽셀 크기
        const FVector2D Size = SizeToCanvas(Cur->ChunkSize);

        FVector2D PosCenter = WorldToCanvas(Cur->GetActorLocation());

        const FVector2D PosBottom = PosCenter + FVector2D(0.f, Size.Y * 0.5f);
        const FVector2D Pivot = { 0.5f, 1.0f };              // 하단-중앙

        URASMapButton* MapButton = CreateWidget<URASMapButton>(GetWorld(), MapButtonClass);
        MapButton->Init(Cur);

        if (auto CanvasSlot = Cast<UCanvasPanelSlot>(MapCanvas->AddChild(MapButton)))
        {
            CanvasSlot->SetAutoSize(false);
            CanvasSlot->SetSize(Size);
            CanvasSlot->SetAlignment(Pivot);  
            CanvasSlot->SetPosition(PosBottom);
        }

        MapButton->SetRenderTransformPivot(Pivot);
        const float RawYaw = Cur->GetActorRotation().Yaw;
        const float SnapYaw = 90.f * FMath::RoundToInt(RawYaw / 90.f);
        MapButton->SetRenderTransformAngle(-SnapYaw);


        for (auto& Door : Cur->GetDoors())
        {
            ARASChunk* NextChunk = Door->GetConnectedChunk();
            if (NextChunk && !Visited.Contains(NextChunk))
            {
                Visited.Add(NextChunk);
                Q.Enqueue(NextChunk);
            }
        }
    }
}

FVector2D URASMapUI::WorldToCanvas(const FVector& P) const
{
    FVector2D WorldPos(P.X, P.Y);

    FVector2D Center = Bounds.GetCenter();
    FVector2D Delta = WorldPos - Center;

    return FVector2D(
        Delta.X * PixelPerUU + MapSize.X * 0.5f,
        -Delta.Y * PixelPerUU + MapSize.Y * 0.5f  
    );
}

FVector2D URASMapUI::SizeToCanvas(const FVector2D& RoomSize) const
{
    return RoomSize * PixelPerUU;
}

