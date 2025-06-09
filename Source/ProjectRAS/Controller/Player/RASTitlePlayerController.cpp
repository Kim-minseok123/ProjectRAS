// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/RASTitlePlayerController.h"
#include "UI/RASTitleWidget.h"

ARASTitlePlayerController::ARASTitlePlayerController()
{

}

void ARASTitlePlayerController::SetTitleUI()
{
	if (TitleWidget)
	{
		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
}

void ARASTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<URASTitleWidget>(this, TitleWidgetClass);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();
			SetTitleUI();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TitleWidgetClass is not set in ARASTitlePlayerController!"));
	}
}
