// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASStatusBarWidget.h"
#include "Component/Stat/RASStatComponent.h"
#include "Components/ProgressBar.h"

URASStatusBarWidget::URASStatusBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASStatusBarWidget::BindHP(class URASStatComponent* InStatComponent)
{
	StatComponent = InStatComponent;
	if (InStatComponent != nullptr)
	{
		InStatComponent->OnHpChanged.AddUObject(this, &URASStatusBarWidget::UpdateHp);
	}
}

void URASStatusBarWidget::BindStamina(class URASStatComponent* InStatComponent)
{
	StatComponent = InStatComponent;
	if (InStatComponent != nullptr)
	{
		InStatComponent->OnStaminaChanged.AddUObject(this, &URASStatusBarWidget::UpdateStamina);
	}
}

void URASStatusBarWidget::UpdateHp(float InHp)
{
    if (StatComponent->IsValidLowLevel() && HP_Bar)
    {
        TargetHPPercent = InHp / StatComponent->GetMaxHp();

        if (FMath::IsNearlyZero(CurrentHPPercent))
        {
            CurrentHPPercent = TargetHPPercent;
            HP_Bar->SetPercent(CurrentHPPercent);
        }
    }
}

void URASStatusBarWidget::UpdateStamina(float InStamina)
{
    if (StatComponent,IsValidLowLevel() && Stamina_Bar)
    {
        TargetStaminaPercent = InStamina / StatComponent->GetMaxStamina();
        if (FMath::IsNearlyZero(CurrentStaminaPercent))
        {
            CurrentStaminaPercent = TargetStaminaPercent;
            Stamina_Bar->SetPercent(CurrentStaminaPercent);
        }
    }
}

void URASStatusBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    float InterpSpeed = 6.0f;

    CurrentHPPercent = FMath::FInterpTo(CurrentHPPercent, TargetHPPercent, InDeltaTime, InterpSpeed);
    if (HP_Bar)
    {
        HP_Bar->SetPercent(CurrentHPPercent);
    }

    CurrentStaminaPercent = FMath::FInterpTo(CurrentStaminaPercent, TargetStaminaPercent, InDeltaTime, InterpSpeed);
    if (Stamina_Bar)
    {
        Stamina_Bar->SetPercent(CurrentStaminaPercent);
    }
}
