// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASPlayerHUDWidget.h"
#include "Component/Stat/RASStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "UMG.h"

URASPlayerHUDWidget::URASPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASPlayerHUDWidget::NativeConstruct()
{
	PlayAnimation(UI_Start, 0.f, 1, EUMGSequencePlayMode::Reverse, 2.f);
}

void URASPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float CurrentHpPercentage = 0;
	float CurrentStaminaPercentage = 0;
	float IntervalTime = 5.f;

	HpOrb->GetDynamicMaterial()->GetScalarParameterValue(TEXT("Percentage"), CurrentHpPercentage);
	StaminaOrb->GetDynamicMaterial()->GetScalarParameterValue(TEXT("Percentage"), CurrentStaminaPercentage);

	if (CurrentHpPercentage != TargetHpPercentage)
	{
		CurrentHpPercentage = FMath::FInterpTo(CurrentHpPercentage, TargetHpPercentage, InDeltaTime, IntervalTime);

		if (HpOrb && HpOrb->GetDynamicMaterial())
		{
			HpOrb->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percentage"), CurrentHpPercentage);
		}
	}
	if (CurrentStaminaPercentage != TargetStaminaPercentage)
	{
		CurrentStaminaPercentage = FMath::FInterpTo(CurrentStaminaPercentage, TargetStaminaPercentage, InDeltaTime, IntervalTime);

		if (StaminaOrb && StaminaOrb->GetDynamicMaterial())
		{
			StaminaOrb->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percentage"), CurrentStaminaPercentage);
		}
	}
}

void URASPlayerHUDWidget::BindHP(class URASStatComponent* InStatComponent)
{
	StatComponent = InStatComponent;
	if (InStatComponent != nullptr)
	{
		InStatComponent->OnHpChanged.AddUObject(this, &URASPlayerHUDWidget::UpdateHp);
	}
}

void URASPlayerHUDWidget::BindStamina(class URASStatComponent* InStatComponent)
{
	StatComponent = InStatComponent;
	if (InStatComponent != nullptr)
	{
		InStatComponent->OnStaminaChanged.AddUObject(this, &URASPlayerHUDWidget::UpdateStamina);
	}
}

void URASPlayerHUDWidget::UpdateHp(float InHp)
{
	if (StatComponent->IsValidLowLevel())
	{
		TargetHpPercentage = InHp / StatComponent->GetMaxHp();
	}
}

void URASPlayerHUDWidget::UpdateStamina(float InStamina)
{
	if (StatComponent->IsValidLowLevel())
	{
		TargetStaminaPercentage = InStamina / StatComponent->GetMaxStamina();
	}
}
