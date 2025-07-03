// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASBossHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASBossHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASBossHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BindHP(class URASStatComponent* InStatComponent);
	void BindStamina(class URASStatComponent* InStatComponent);

protected:

	UFUNCTION()
	void UpdateHp(float InHp);
	UFUNCTION()
	void UpdateStamina(float InStamina);

	UFUNCTION()
	void HideStaminaBar();

	TWeakObjectPtr<class URASStatComponent> StatComponent;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HP_Bar;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> Stamina_Bar;

	float CurrentHPPercent = 0.f;
	float TargetHPPercent = 0.f;

	float CurrentStaminaPercent = 0.f;
	float TargetStaminaPercent = 0.f;

	FTimerHandle StaminaTimerHandle;
};
