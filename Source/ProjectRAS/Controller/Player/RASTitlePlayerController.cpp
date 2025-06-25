// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/RASTitlePlayerController.h"
#include "UI/RASTitleWidget.h"
#include "Audio/RASAudioSubsystem.h"
#include "GameFramework/GameUserSettings.h"

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
	UGameUserSettings* S = GEngine->GetGameUserSettings();
	S->SetScreenResolution({ 1920,1080 });
	S->SetFullscreenMode(EWindowMode::Fullscreen);  
	S->ApplySettings(false);


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

	GetGameInstance()->GetSubsystem<URASAudioSubsystem>()->PlayBGM(TEXT("Title"), 0.5f, 0.5f);
}
