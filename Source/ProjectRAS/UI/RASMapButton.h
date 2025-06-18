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


	void Init(class ARASChunk* InChunk);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MapButton;
};
