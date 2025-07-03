// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/RASNpc.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASCombatComponent.h"
#include "Component/Player/RASUIComponent.h"

ARASNpc::ARASNpc()
{
	NPCInteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCInteractionWidget"));
	NPCInteractionWidget->SetupAttachment(RootComponent);
	

	NPCInteractionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("NPCInteractionCamera"));
	NPCInteractionCamera->SetupAttachment(RootComponent);

}

void ARASNpc::InteractionUIOn()
{
	if (!NPCInteractionWidget->IsVisible())
	{
		NPCInteractionWidget->SetVisibility(true);
	}
}

void ARASNpc::InteractionUIOff()
{
	if (NPCInteractionWidget->IsVisible())
		NPCInteractionWidget->SetVisibility(false);
}

void ARASNpc::InteractionWithPlayer()
{
	if (bIsHealPotion) return;
	
	FVector PlayerLocation = Player->GetActorLocation();
	FVector NpcLocation = GetActorLocation();
	float Distance = FVector::Dist(PlayerLocation, NpcLocation);

	if (Distance < 200.f)
	{
		bIsInteracting = true;
		InteractionUIOff();
		// 카메라 전환
		if (NPCInteractionCamera)
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			OriginalViewTarget = PlayerController->GetViewTarget();
			if (PlayerController)
			{
				PlayerController->SetViewTargetWithBlend(this, 0.5f);
			}
		}
		Player->GetUIComponent()->ShowNpcUI(this);
	}
}

void ARASNpc::InteractionWithPlayerEnd()
{
	bIsInteracting = false;
	// 카메라 복원
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(OriginalViewTarget.Get(), 0.5f);
}

void ARASNpc::BeginPlay()
{
	Super::BeginPlay();
	ARASPlayer* FirstPlayer = Cast<ARASPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (FirstPlayer)
	{
		Player = FirstPlayer;
	}
}

void ARASNpc::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsInteracting || bIsHealPotion)
	{
		return;
	}
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NpcLocation = GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, NpcLocation);

		if (Distance < 200.f)
		{
			InteractionUIOn();
		}
		else
		{
			InteractionUIOff();
		}
	}
}

void ARASNpc::AcceptInteraction()
{
	Player->GetUIComponent()->HideNpcUI();
	Player->GetCombatComponent()->RecoverPotion();
	bIsHealPotion = true;
	InteractionWithPlayerEnd();
}

void ARASNpc::CancelInteraction()
{
	Player->GetUIComponent()->HideNpcUI();
	InteractionWithPlayerEnd();
}
