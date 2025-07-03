// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASTitleWidget.h"
#include "UI/RASUISubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

URASTitleWidget::URASTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UI_StartGame)
	{
		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindDynamic(this, &URASTitleWidget::OnStartGameEnd);
		BindToAnimationFinished(UI_StartGame, AnimationFinishedEvent);

		PlayAnimation(UI_StartGame);
	}
	if (StartButton)
	{
		StartButton->SetIsEnabled(false);
		
	}
	if (MenuButton)
	{
		MenuButton->OnClicked.AddDynamic(this, &URASTitleWidget::OnMenuWidget);
	}
}

void URASTitleWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("L_Dungeons"));
}

void URASTitleWidget::OnStartGameEnd()
{
	if (StartButton)
	{
		StartButton->SetIsEnabled(true);
	}
	if (UI_TextAnim)
	{
		PlayAnimation(UI_TextAnim, 0.f, 0);
	}
}

void URASTitleWidget::OnMenuWidget()
{
	URASUISubsystem* UISubsystem = GetGameInstance()->GetSubsystem<URASUISubsystem>();
	if (UISubsystem)
	{
		UISubsystem->ShowMenu();
	}
}
