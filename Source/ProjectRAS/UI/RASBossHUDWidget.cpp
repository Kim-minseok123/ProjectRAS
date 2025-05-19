// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASBossHUDWidget.h"
#include "Component/Stat/RASStatComponent.h"
#include "Components/ProgressBar.h"

URASBossHUDWidget::URASBossHUDWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void URASBossHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HP_Bar->SetPercent(1);

    Stamina_Bar->SetPercent(1);
}

void URASBossHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void URASBossHUDWidget::BindHP(class URASStatComponent* InStatComponent)
{
    StatComponent = InStatComponent;
    if (InStatComponent != nullptr)
    {
        InStatComponent->OnHpChanged.AddUObject(this, &URASBossHUDWidget::UpdateHp);
    }
}

void URASBossHUDWidget::BindStamina(class URASStatComponent* InStatComponent)
{
    StatComponent = InStatComponent;
    if (InStatComponent != nullptr)
    {
        InStatComponent->OnStaminaChanged.AddUObject(this, &URASBossHUDWidget::UpdateStamina);
        InStatComponent->OnStaminaChanged.AddUObject(this, &URASBossHUDWidget::UpdateStaminaHandle);
    }
}

void URASBossHUDWidget::UpdateHp(float InHp)
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

void URASBossHUDWidget::UpdateStamina(float InStamina)
{
    if (StatComponent->IsValidLowLevel() && Stamina_Bar)
    {
        Stamina_Bar->SetVisibility(ESlateVisibility::Visible);
        TargetStaminaPercent = InStamina / StatComponent->GetMaxStamina();
        if (FMath::IsNearlyZero(CurrentStaminaPercent))
        {
            CurrentStaminaPercent = TargetStaminaPercent;
            Stamina_Bar->SetPercent(CurrentStaminaPercent);
        }
    }
}

void URASBossHUDWidget::HideStaminaBar()
{
    Stamina_Bar->SetVisibility(ESlateVisibility::Collapsed);
}

void URASBossHUDWidget::UpdateStaminaHandle(float InStamina)
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, [this]()
        {
            HideStaminaBar();
        }, 3.f, false);
}
