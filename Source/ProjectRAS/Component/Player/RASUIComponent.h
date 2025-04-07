// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RASUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URASUIComponent();

	void InitUI();

	UFUNCTION(BlueprintCallable, Category="UI")
	bool ShowHUD();

	UFUNCTION(BlueprintCallable, Category="UI")
	bool HideHUD();

protected:


	UPROPERTY(EditAnywhere)
	TSubclassOf<class URASPlayerHUDWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class URASPlayerHUDWidget> PlayerHUDWidget;

public:	
};
