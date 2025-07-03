// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/RASPlayerController.h"
#include "UI/RASUISubsystem.h"
#include "Audio/RASAudioSubsystem.h"
#include "UI/RASMenuWidget.h"

ARASPlayerController::ARASPlayerController()
{

}

void ARASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	URASUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<URASUISubsystem>();
	if (UISubsystem) 
	{
		UISubsystem->FadeOut();
	}
	if (UISubsystem)
	{
		UISubsystem->ShowMenu();
		UISubsystem->HideMenu();
	}
	UISubsystem->SetVolume();
	GetGameInstance()->GetSubsystem<URASAudioSubsystem>()->PlayBGM(TEXT("Main"), 0.5f, 0.5f);
}
