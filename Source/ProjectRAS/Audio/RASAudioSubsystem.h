// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RASAudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	URASAudioSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void PlayBGM(FName Key, float FadeOutSec = 1.0f, float FadeInSec = 1.0f);
	void PlaySFX(FName Key, const FVector& PlayPos, float Volume = 1.0f);

protected:
	UPROPERTY()
	TObjectPtr<class URASSoundData> SoundData;

	UPROPERTY()
	TObjectPtr<class UAudioComponent> CurrentBGM;

	class USoundCue* GetCue(const TMap<FName, TSoftObjectPtr<USoundCue>>& Map, FName Key);
};
