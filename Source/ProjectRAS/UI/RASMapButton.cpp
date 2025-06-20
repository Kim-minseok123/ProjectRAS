// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMapButton.h"
#include "Map/RASChunk.h"
#include "Character/Player/RASPlayer.h"
#include "Components/Button.h"
#include "Data/RASMapType.h"
#include "Map/RASCorridor.h"
URASMapButton::URASMapButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASMapButton::Init(class ARASChunk* InChunk, class ARASPlayer* InPlayer)
{
	if (InChunk) Chunk = InChunk;
	if (InPlayer) Player = InPlayer;
	if (InChunk && InChunk->GetActorLocation() == FVector::ZeroVector)
	{
		MapButton->WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor::Green);
		MapButton->WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor::Green);
		MapButton->WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor::Green);
	}

	if (InChunk->GetMapType() == ERASMapType::Corridor)
	{
		ARASCorridor* Corridor = Cast<ARASCorridor>(Chunk);
		if (Corridor)
		{
			if (Corridor->GetCorridorType() == ERASCorridorType::BentLeft)
			{
				if (LeftCorridorImage)
				{
					SetButtonImage(LeftCorridorImage);
				}
			}
			else if (Corridor->GetCorridorType() == ERASCorridorType::BentRight)
			{
				if (RightCorridorImage)
				{
					SetButtonImage(RightCorridorImage);
				}
			}
		}
	}
}

void URASMapButton::OnButtonClicked()
{
	if (Chunk && Player)
	{
		UE_LOG(LogTemp, Log, TEXT("Select Location : %s"), *(Chunk->GetActorLocation().ToString()));
		if (Player->GetCurrentChunk() == Chunk) return;
		// 맵 순간이동
		Player->TeleportToChunk(Chunk);
	}
}

bool URASMapButton::CheckVisitChunk()
{
	if (Chunk)
	{
		return Chunk->IsArrive();
	}
	return false;
}

void URASMapButton::SetCurrentChunk()
{
	if (!MapButton || !Chunk) return;

	FLinearColor NormalColor;
	FLinearColor HoveredColor;
	FLinearColor PressedColor;

	if (CheckCurrentChunk())
	{
		NormalColor = FLinearColor::Red;
		HoveredColor = FLinearColor(1.f, 0.f, 0.f, 0.8f);   
		PressedColor = FLinearColor(1.f, 0.f, 0.f, 0.6f);  
	}
	else
	{
		if (Chunk->GetActorLocation() == FVector::ZeroVector)
		{
			NormalColor = FLinearColor::Green;
			HoveredColor = FLinearColor(0.f, 1.f, 0.f, 0.8f);
			PressedColor = FLinearColor(0.f, 1.f, 0.f, 0.6f);
		}
		else
		{
			NormalColor = FLinearColor::White;
			HoveredColor = FLinearColor(1.f, 1.f, 1.f, 0.8f);
			PressedColor = FLinearColor(1.f, 1.f, 1.f, 0.6f);
		}
	}

	FButtonStyle NewStyle = MapButton->WidgetStyle;
	NewStyle.Normal.TintColor = FSlateColor(NormalColor);
	NewStyle.Hovered.TintColor = FSlateColor(HoveredColor);
	NewStyle.Pressed.TintColor = FSlateColor(PressedColor);

	MapButton->SetStyle(NewStyle);
}

void URASMapButton::SetButtonImage(TObjectPtr<class UTexture2D> MyTexture)
{
	FSlateBrush NormalBrush;
	NormalBrush.SetResourceObject(MyTexture);
	NormalBrush.ImageSize = FVector2D(MyTexture->GetSizeX(), MyTexture->GetSizeY());

	FButtonStyle ButtonStyle = MapButton->WidgetStyle;
	ButtonStyle.Normal = NormalBrush;

	FSlateBrush HoveredBrush = NormalBrush;
	HoveredBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.8f);
	ButtonStyle.Hovered = NormalBrush;

	FSlateBrush PressedBrush = NormalBrush;
	PressedBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.6f);
	ButtonStyle.Pressed = NormalBrush;

	MapButton->SetStyle(ButtonStyle);
}

bool URASMapButton::CheckCurrentChunk()
{
	if (Chunk && Player) 
	{
		return Player->GetCurrentChunk() == Chunk;
	}
	return false;
}
