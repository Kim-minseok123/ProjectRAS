// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASMenuWidget.h"
#include "UI/RASUISubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"

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
	if (BGMSlider)
	{
		BGMSlider->OnValueChanged.AddDynamic(this, &URASMenuWidget::SetBGMVolume);
	}
	if (SFXSlider)
	{
		SFXSlider->OnValueChanged.AddDynamic(this, &URASMenuWidget::SetSFXVolume);
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
	Value = FMath::Clamp(Value, 0.0f, 1.0f);
	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		SoundMix,
		BGMClass,
		Value,
		1.f,
		0.5f
	);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}

void URASMenuWidget::SetSFXVolume(float Value)
{
	Value = FMath::Clamp(Value, 0.0f, 1.0f);
	UGameplayStatics::SetSoundMixClassOverride(
		GetWorld(),
		SoundMix,
		SFXClass,
		Value,
		1.f,
		0.5f
	);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}
