// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASUISubsystem.h"
#include "UI/RASMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/Player/RASTitlePlayerController.h"
#include "UI/RASFadeWidget.h"

URASUISubsystem::URASUISubsystem()
{
	static ConstructorHelpers::FClassFinder<URASMenuWidget> MenuWidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_Menu.WBP_Menu_C'"));
	if (MenuWidgetBPClass.Class)
	{
		MenuWidgetClass = MenuWidgetBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<URASFadeWidget> FadeWidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_RASFadeWidget.WBP_RASFadeWidget_C'"));
	if (FadeWidgetBPClass.Class)
	{
		FadeWidgetClass = FadeWidgetBPClass.Class;
	}
}

void URASUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URASUISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void URASUISubsystem::ShowMenu()
{
	if (!MenuWidget && MenuWidgetClass)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		MenuWidget = CreateWidget<URASMenuWidget>(
			/*Outer=*/GetGameInstance(),   
			MenuWidgetClass);
	}
	if (!MenuWidget) return; 
	if (!MenuWidget->IsInViewport())
	{
		MenuWidget->AddToViewport(100);
	}
	if (MenuWidget)
	{
		MenuWidget->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MenuWidget->GetCachedWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void URASUISubsystem::HideMenu()
{
	if (MenuWidget)
	{
		MenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		ARASTitlePlayerController* TitlePC = Cast<ARASTitlePlayerController>(PC);
		if (TitlePC)
		{
			TitlePC->SetTitleUI();
		}
		else
		{
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		}

	}
}

void URASUISubsystem::FadeIn()
{
	if (FadeWidget)
	{
		FadeWidget->RemoveFromParent();
		FadeWidget = nullptr;
	}

	if (FadeWidgetClass)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		FadeWidget = CreateWidget<URASFadeWidget>(GetGameInstance(), FadeWidgetClass);
		if (FadeWidget)
		{
			FadeWidget->AddToViewport(500);
			FadeWidget->FadeIn();
		}
	}
}

void URASUISubsystem::FadeOut()
{
	if (FadeWidget)
	{
		FadeWidget->RemoveFromParent();
		FadeWidget = nullptr;
	}

	if (FadeWidgetClass)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		FadeWidget = CreateWidget<URASFadeWidget>(GetGameInstance(), FadeWidgetClass);
		if (FadeWidget)
		{
			FadeWidget->AddToViewport(500);
			FadeWidget->FadeOut();
		}
	}
}

void URASUISubsystem::SetVolume()
{
	if (MenuWidget)
	{
		MenuWidget->SetBGMVolume(BGMVolume);
		MenuWidget->SetSFXVolume(SFXVolume);
	}
}

bool URASUISubsystem::IsMenuVisible() const
{
	return MenuWidget && MenuWidget->IsVisible();
}
