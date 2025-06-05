// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASTitleWidget.h"

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
}

void URASTitleWidget::StartGame()
{

}

void URASTitleWidget::OnStartGameEnd()
{
	if (UI_TextAnim)
	{
		PlayAnimation(UI_TextAnim, 0.f, 0);
	}
}

void URASTitleWidget::ExitGame()
{

}
