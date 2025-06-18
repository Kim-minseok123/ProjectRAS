// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASUIComponent.h"
#include "UI/RASPlayerHUDWidget.h"
#include "Component/Stat/RASStatComponent.h"
#include "Character/Player/RASPlayer.h"
#include "UI/RASBossHUDWidget.h"
#include "UI/RASMapUI.h"
#include "Kismet/GameplayStatics.h"

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

void URASUIComponent::SetIconProgressBar(int InIdx, float InDuration, int Count /*= 5*/)
{
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->SetIconProgressBar(InIdx, InDuration, Count);
	}
}

void URASUIComponent::EnterBattle()
{
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->HideMiniMap();
	}

}

void URASUIComponent::ExitBattle()
{
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->ShowMiniMap();
	}
}

bool URASUIComponent::ShowMapUI()
{
	if (!MapUI && MapUIClass)
	{
		MapUI = CreateWidget<URASMapUI>(GetWorld(), MapUIClass);
	}
	if (MapUI && !MapUI->IsInViewport())
	{
		MapUI->AddToViewport(10);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MapUI->GetCachedWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
		return true;
	}
	return false;
}

bool URASUIComponent::HideMapUI()
{
	if (MapUI && MapUI->IsInViewport())
	{
		MapUI->RemoveFromViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
		return true;
	}
	return false;
}



void URASUIComponent::SetMapUI(FBox2D InBound, TArray<TObjectPtr<class ARASChunk>>& SpawnChunks)
{
	if (!MapUI && MapUIClass)
	{
		MapUI = CreateWidget<URASMapUI>(GetWorld(), MapUIClass);
	}
	MapUI->InitMap(InBound);
	MapUI->BuildMapUI(SpawnChunks);
}

