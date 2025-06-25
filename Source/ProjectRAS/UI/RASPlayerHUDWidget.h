// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	URASPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void BindHP(class URASStatComponent* InStatComponent);
	void BindStamina(class URASStatComponent* InStatComponent);

	void SetIconProgressBar(int InIdx, float InDuration, int Count = 5);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideMiniMap();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMiniMap();

protected:

	UFUNCTION()
	void UpdateHp(float InHp);
	UFUNCTION()
	void UpdateStamina(float InStamina);
protected:
	float TargetHpPercentage;
	float TargetStaminaPercentage;
	
	TWeakObjectPtr<class URASStatComponent> StatComponent;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> HpOrb;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> StaminaOrb;
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<class UWidgetAnimation> UI_Start;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> SkillBar1;
	FTimerHandle SkillBar1Handle;
	float ElapsedTime1 = 0.0f;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> SkillBar2;
	FTimerHandle SkillBar2Handle;
	float ElapsedTime2 = 0.0f;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> ItemBar1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> PotionCountTxt;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> MiniMap;

	FTimerHandle ItemBar1Handle;
	float ElapsedTime3 = 0.0f;
};
