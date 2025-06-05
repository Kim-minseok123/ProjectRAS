// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RASTitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARASTitlePlayerController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class URASTitleWidget> TitleWidgetClass;

	UPROPERTY()
	TObjectPtr<class URASTitleWidget> TitleWidget;

	virtual void BeginPlay() override;
};
