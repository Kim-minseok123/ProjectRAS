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



void URASPlayerHUDWidget::SetIconProgressBar(int InIdx, float InDuration, int Count /*= 5*/)
{
	const float LocalDuration = InDuration;
	switch (InIdx)
	{
	case 1:
		ElapsedTime1 = 0.f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SkillBar1Handle,
				[this, LocalDuration]()
				{
					ElapsedTime1 += 0.01f;

					float Percent = 1.0f - FMath::Clamp(ElapsedTime1 / LocalDuration, 0.0f, 1.0f);
					if (SkillBar1)
					{
						SkillBar1->SetPercent(Percent);
					}

					if (ElapsedTime1 >= LocalDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(SkillBar1Handle);
					}
				},
				0.01f,
				true
			);
		}
		break;
	case 2:
		ElapsedTime2 = 0.f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SkillBar2Handle,
				[this, LocalDuration]()
				{
					ElapsedTime2 += 0.01f;

					float Percent = 1.0f - FMath::Clamp(ElapsedTime2 / LocalDuration, 0.0f, 1.0f);
					if (SkillBar2)
					{
						SkillBar2->SetPercent(Percent);
					}

					if (ElapsedTime2 >= LocalDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(SkillBar2Handle);
					}
				},
				0.01f,
				true
			);
		}
		break;
	case 3:
		PotionCountTxt->SetText(FText::AsNumber(Count));
		ElapsedTime3 = 0.f;
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				ItemBar1Handle,
				[this, LocalDuration]()
				{
					ElapsedTime3 += 0.01f;

					float Percent = 1.0f - FMath::Clamp(ElapsedTime3 / LocalDuration, 0.0f, 1.0f);
					if (ItemBar1)
					{
						ItemBar1->SetPercent(Percent);
					}

					if (ElapsedTime3 >= LocalDuration)
					{
						GetWorld()->GetTimerManager().ClearTimer(ItemBar1Handle);
					}
				},
				0.01f,
				true
			);
		}
		break;
	}

}

void URASPlayerHUDWidget::HideMiniMap()
{
	if (MiniMap)
	{
		MiniMap->SetVisibility(ESlateVisibility::Hidden);
	}

}

void URASPlayerHUDWidget::ShowMiniMap()
{
	if (MiniMap)
	{
		MiniMap->SetVisibility(ESlateVisibility::Visible);
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
