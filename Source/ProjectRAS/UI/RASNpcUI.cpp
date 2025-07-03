// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASNpcUI.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Character/Npc/RASNpc.h"
#include "Audio/RASAudioSubsystem.h"

URASNpcUI::URASNpcUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASNpcUI::Setup(class ARASNpc* InOnwerNpc)
{
    OwnerNpc = InOnwerNpc;
    if (NpcText)
	{
		NpcText->SetText(FText::FromString(TEXT("")));
	}
    if (YesButton)
	{
		YesButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (NoButton)
	{
		NoButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void URASNpcUI::StartTyping(const FString& InText, float InSpeed)
{
    FullText = InText;
    FullText.ReplaceInline(TEXT("\\n"), TEXT("\n"));
    CurrentIndex = 0;
    TypingSpeed = InSpeed;
    GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &URASNpcUI::AddNextCharacter, TypingSpeed, true);
}

void URASNpcUI::AddNextCharacter()
{
    if (CurrentIndex < FullText.Len())
    {
        FString Sub = FullText.Left(CurrentIndex + 1);
        NpcText->SetText(FText::FromString(Sub));
        GetGameInstance()->GetSubsystem<URASAudioSubsystem>()->PlaySFX(TEXT("Typing"), OwnerNpc->GetActorLocation());
        CurrentIndex++;
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        YesButton->SetVisibility(ESlateVisibility::Visible);
        NoButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void URASNpcUI::ClickYes()
{
    if (OwnerNpc)
    {
        OwnerNpc->AcceptInteraction();
    }
}

void URASNpcUI::ClickNo()
{
	if (OwnerNpc)
	{
		OwnerNpc->CancelInteraction();
	}
}
