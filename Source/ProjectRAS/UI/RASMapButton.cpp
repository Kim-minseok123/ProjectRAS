// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMapButton.h"
#include "Map/RASChunk.h"
#include "Character/Player/RASPlayer.h"

URASMapButton::URASMapButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASMapButton::Init(class ARASChunk* InChunk, class ARASPlayer* InPlayer)
{
	if (InChunk) Chunk = InChunk;
	if (InPlayer) Player = InPlayer;
}

void URASMapButton::OnButtonClicked()
{
	if (Chunk && Player)
	{
		if (Player->GetCurrentChunk() == Chunk) return;
		// 맵 순간이동
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
	if (CheckCurrentChunk())
	{
		// 색상 변경
	}
}

bool URASMapButton::CheckCurrentChunk()
{
	if (Chunk && Player) 
	{
		return Player->GetCurrentChunk() == Chunk;
	}
	return false;
}
