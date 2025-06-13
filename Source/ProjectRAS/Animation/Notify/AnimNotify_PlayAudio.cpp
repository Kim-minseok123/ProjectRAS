// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_PlayAudio.h"
#include "Audio/RASAudioSubsystem.h"

UAnimNotify_PlayAudio::UAnimNotify_PlayAudio()
{

}

void UAnimNotify_PlayAudio::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if(!MeshComp) return;

	UWorld* World = MeshComp->GetWorld();
	if (!World) return;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return;

	URASAudioSubsystem* AudioSubsystem = GameInstance->GetSubsystem<URASAudioSubsystem>();
	if (AudioSubsystem)
	{
		AudioSubsystem->PlaySFX(AudioName, MeshComp->GetComponentLocation());
	}
}
