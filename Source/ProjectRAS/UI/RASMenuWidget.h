// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OffMenuWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ExitGame();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetBGMVolume(float Value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetSFXVolume(float Value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeMode();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> OffButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> BGMSlider;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> SFXSlider;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCheckBox> ModeCheckBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundMix> SoundMix;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundClass> BGMClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundClass> SFXClass;

};	
