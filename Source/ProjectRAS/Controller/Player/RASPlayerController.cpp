// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/RASPlayerController.h"
#include "UI/RASUISubsystem.h"


ARASPlayerController::ARASPlayerController()
{

}

void ARASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	URASUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<URASUISubsystem>();
	if (UISubsystem)
	{
		UISubsystem->ShowMenu();
		UISubsystem->HideMenu();
	}
}
