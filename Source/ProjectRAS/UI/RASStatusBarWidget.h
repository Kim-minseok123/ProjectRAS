// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASStatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASStatusBarWidget(const FObjectInitializer& ObjectInitializer);

	void BindHP(class URASStatComponent* InStatComponent);
	void BindStamina(class URASStatComponent* InStatComponent);

protected:
	UFUNCTION()
	void UpdateHp(float InHp);
	UFUNCTION()
	void UpdateStamina(float InStamina);

	TWeakObjectPtr<class URASStatComponent> StatComponent;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HP_Bar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> Stamina_Bar;
};
