// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "RASNpc.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASNpc : public ARASCharacterBase
{
	GENERATED_BODY()
public:
	ARASNpc();

	void InteractionUIOn();
	void InteractionUIOff();

	void InteractionWithPlayer();
	void InteractionWithPlayerEnd();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> NPCInteractionWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> NPCInteractionCamera;

	TObjectPtr<class ARASPlayer> Player;

	TWeakObjectPtr<AActor> OriginalViewTarget;

	bool bIsInteracting = false;

	bool bIsHealPotion = false;
};
