// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASPlayerDeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASPlayerDeathWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="UI")
	void RestartGame();
	UFUNCTION(BlueprintCallable, Category="UI")
	void QuitGame();
};
