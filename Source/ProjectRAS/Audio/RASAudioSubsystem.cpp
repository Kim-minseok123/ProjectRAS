// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/RASAudioSubsystem.h"
#include "Data/RASSoundData.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

URASAudioSubsystem::URASAudioSubsystem()
{
	static ConstructorHelpers::FObjectFinder<URASSoundData> SoundDataObj(TEXT("/Script/ProjectRAS.RASSoundData'/Game/1_ProjectRAS/Data/DA_RASAudioData.DA_RASAudioData'"));
	if (SoundDataObj.Object)
	{
		SoundData = SoundDataObj.Object;
	}
}

class USoundCue* URASAudioSubsystem::GetCue(const TMap<FName, TSoftObjectPtr<USoundCue>>& Map, FName Key)
{
	if (const TSoftObjectPtr<USoundCue>* SoundPtr = Map.Find(Key))
	{
		return SoundPtr->LoadSynchronous();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound cue with key '%s' not found."), *Key.ToString());
		return nullptr;
	}

}
void URASAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URASAudioSubsystem::PlayBGM(FName Key, float FadeOutSec /*= 1.0f*/, float FadeInSec /*= 1.0f*/)
{
	if (SoundData == nullptr) return;
	if (USoundCue* SoundCue = GetCue(SoundData->BGMMap, Key))
	{
		if (CurrentBGM && CurrentBGM->IsPlaying())
		{
			CurrentBGM->FadeOut(FadeOutSec, 0.f);
		}
		CurrentBGM = UGameplayStatics::CreateSound2D(
			GetWorld(),
			SoundCue,
			1.0f, 
			1.0f, 
			0.0f, 
			nullptr,
			true,
			false
		);

		if(CurrentBGM)
		{
			CurrentBGM->FadeIn(FadeInSec, 1.0f);
		}
		
	}
}

void URASAudioSubsystem::PlaySFX(FName Key, const FVector& PlayPos, float Volume /*= 1.0f*/)
{
	if (SoundData == nullptr) return;
	if (USoundCue* SoundCue = GetCue(SoundData->SFXMap, Key))
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			SoundCue,
			PlayPos,
			Volume
		);
	}
}

