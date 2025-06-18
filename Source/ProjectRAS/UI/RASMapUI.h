// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASMapUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASMapUI : public UUserWidget
{
	GENERATED_BODY()
public:
	URASMapUI(const FObjectInitializer& ObjectInitializer);

	void InitMap(FBox2D InBox);

	void BuildMapUI(const TArray<TObjectPtr<class ARASChunk>>& Spawned);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> MapCanvas;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASMapButton> MapButtonClass;

	FBox2D Bounds;
	FVector2D MapSize;
	float PixelPerUU;

	FVector2D WorldToCanvas(const FVector& P) const;
	FVector2D SizeToCanvas(const FVector2D& RoomSize) const;
};
