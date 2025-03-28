// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASAimWidget.h"
#include "Components/Image.h"

URASAimWidget::URASAimWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void URASAimWidget::SetupAim()
{
	Aim->SetVisibility(ESlateVisibility::Collapsed);
	Last_Aim->SetVisibility(ESlateVisibility::Collapsed);
}

void URASAimWidget::VisibleAim(bool bVisible)
{
	if(bVisible)
	{
		Aim->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Aim->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URASAimWidget::VisibleLastAim(bool bVisible)
{
	if(bVisible)
	{
		Last_Aim->SetVisibility(ESlateVisibility::Visible);
		Aim->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Last_Aim->SetVisibility(ESlateVisibility::Collapsed);
		Aim->SetVisibility(ESlateVisibility::Visible);
	}
}

void URASAimWidget::VisibleAllAim(bool bVisible)
{
	if (bVisible)
	{
		Last_Aim->SetVisibility(ESlateVisibility::Visible);
		Aim->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Last_Aim->SetVisibility(ESlateVisibility::Collapsed);
		Aim->SetVisibility(ESlateVisibility::Collapsed);
	}
}
