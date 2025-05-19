// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASUIComponent.h"
#include "UI/RASPlayerHUDWidget.h"
#include "Component/Stat/RASStatComponent.h"
#include "Character/Player/RASPlayer.h"
#include "UI/RASBossHUDWidget.h"

// Sets default values for this component's properties
URASUIComponent::URASUIComponent()
{
	static ConstructorHelpers::FClassFinder<URASPlayerHUDWidget> PlayerHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (PlayerHUDWidgetRef.Class)
	{
		PlayerHUDWidgetClass = PlayerHUDWidgetRef.Class;
	}

}

void URASUIComponent::InitUI()
{
	if (ShowHUD() == false)
	{
		ensure(false);
		return;
	}

	ARASPlayer* Player = Cast<ARASPlayer>(GetOwner());
	URASStatComponent* Stat = Player->GetStat();

	if (!Stat)
	{
		ensure(false);
		return;
	}

	PlayerHUDWidget->BindHP(Stat);
	PlayerHUDWidget->BindStamina(Stat);
}

bool URASUIComponent::ShowHUD()
{
	if (!PlayerHUDWidget && PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<URASPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
	}
	if (PlayerHUDWidget && !PlayerHUDWidget->IsInViewport())
	{
		PlayerHUDWidget->AddToViewport();
		return true;
	}
	return false;
}

bool URASUIComponent::HideHUD()
{
	if (PlayerHUDWidget && PlayerHUDWidget->IsInViewport())
	{
		PlayerHUDWidget->RemoveFromViewport();
		return true;
	}
	return false;
}

