// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMenuWidget.h"
#include "UI/RASUISubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

URASMenuWidget::URASMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &URASMenuWidget::ExitGame);
	}
	if (OffButton)
	{
		OffButton->OnClicked.AddDynamic(this, &URASMenuWidget::OffMenuWidget);
	}
}

void URASMenuWidget::OffMenuWidget()
{
	URASUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<URASUISubsystem>();
	if (UISubsystem)
	{
		UISubsystem->HideMenu();
	}
}

void URASMenuWidget::ExitGame()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();   
	UKismetSystemLibrary::QuitGame(
		this,                     
		PC,                       
		EQuitPreference::Quit,    
		false                     
	);
}

void URASMenuWidget::SetBGMVolume(float Value)
{

}

void URASMenuWidget::SetSFXVolume(float Value)
{

}
