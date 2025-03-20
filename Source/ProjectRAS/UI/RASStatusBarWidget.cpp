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
	if (StatComponent->IsValidLowLevel()) 
	{
		HP_Bar->SetPercent(InHp / StatComponent->GetMaxHp());
	}
}

void URASStatusBarWidget::UpdateStamina(float InStamina)
{
	if (StatComponent->IsValidLowLevel())
	{
		Stamina_Bar->SetPercent(InStamina / StatComponent->GetMaxStamina());
	}
}
