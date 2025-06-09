// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASTitleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASTitleWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void StartGame();

	UFUNCTION()
	void OnStartGameEnd();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnMenuWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> MenuButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> UI_StartGame;
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> UI_TextAnim;
};
