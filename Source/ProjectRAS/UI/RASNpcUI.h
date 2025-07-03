// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RASNpcUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASNpcUI : public UUserWidget
{
	GENERATED_BODY()
public:
	URASNpcUI(const FObjectInitializer& ObjectInitializer);
	void Setup(class ARASNpc* InOnwerNpc);
	void StartTyping(const FString& InText, float InSpeed);
	void AddNextCharacter();

	UFUNCTION(BlueprintCallable, Category = "NPC UI")
	void ClickYes();
	UFUNCTION(BlueprintCallable, Category = "NPC UI")
	void ClickNo();

protected:

	FTimerHandle TypingTimerHandle;
	FString FullText;
	int32 CurrentIndex = 0;
	float TypingSpeed = 0.05f;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> NpcText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> YesButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> NoButton;

	TObjectPtr<class ARASNpc> OwnerNpc;
};
