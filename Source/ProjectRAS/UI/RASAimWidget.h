// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASAimWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASAimWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASAimWidget(const FObjectInitializer& ObjectInitializer);

	void SetupAim();
	UFUNCTION()
	void VisibleAim(bool bVisible);
	UFUNCTION()
	void VisibleLastAim(bool bVisible);
	void VisibleAllAim(bool bVisible);
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Aim;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> Last_Aim;
};
