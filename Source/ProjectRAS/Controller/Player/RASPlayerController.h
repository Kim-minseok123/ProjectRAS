// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARASPlayerController();

	virtual void BeginPlay() override;
};
