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

	void BuildMapUI(const TArray<TObjectPtr<class ARASChunk>>& Spawned, class ARASChunk* StartChunk, class ARASPlayer* InPlayer);

	void FoundMapShow();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void ExitButtonClick();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> MapCanvas;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> VerScrollBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> HorScrollBox;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class USizeBox> ScrollSizeBox;
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASMapButton> MapButtonClass;

	TArray<TObjectPtr<class URASMapButton>> MapButtons;

	TObjectPtr<class ARASPlayer> Player;

	FIntPoint GetCellSize(const class  ARASChunk* Chunk) const;

	float RAS_CellWorldUnit = 300.f;  
	float RAS_CellPixel = 64.f;
};
