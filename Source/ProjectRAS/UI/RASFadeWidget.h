// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASFadeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASFadeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URASFadeWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void FadeIn();
	UFUNCTION(BlueprintCallable)
	void FadeOut();

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<class UWidgetAnimation> FadeInAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> FadeOutAnim;
};
