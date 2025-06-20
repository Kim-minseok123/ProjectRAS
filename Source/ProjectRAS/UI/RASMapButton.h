// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RASMapType.h"
#include "RASMapButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASMapButton : public UUserWidget
{
	GENERATED_BODY()
public:
	URASMapButton(const FObjectInitializer& ObjectInitializer);


	void Init(class ARASChunk* InChunk, class ARASPlayer* InPlayer);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnButtonClicked();

	bool CheckVisitChunk();

	void SetCurrentChunk();

	void SetButtonImage(TObjectPtr<class UTexture2D> MyTexture);

	bool CheckCurrentChunk();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MapButton;

	UPROPERTY()
	TObjectPtr<class ARASChunk> Chunk;

	UPROPERTY()
	TObjectPtr<class ARASPlayer> Player;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> LeftCorridorImage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> RightCorridorImage;
};
