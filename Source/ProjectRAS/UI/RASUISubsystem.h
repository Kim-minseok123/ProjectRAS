// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RASUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	URASUISubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable) 
	void ShowMenu();
	UFUNCTION(BlueprintCallable)
	void HideMenu();

	UFUNCTION(BlueprintCallable)
	void FadeIn();
	UFUNCTION(BlueprintCallable)
	void FadeOut();

	void SetVolume();

	bool IsMenuVisible() const;

	float BGMVolume = 0.5f;
	float SFXVolume = 0.5f;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URASMenuWidget> MenuWidgetClass;
	UPROPERTY()
	TObjectPtr<class URASMenuWidget> MenuWidget;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URASFadeWidget> FadeWidgetClass;
	UPROPERTY()
	TObjectPtr<class URASFadeWidget> FadeWidget;
	
};
