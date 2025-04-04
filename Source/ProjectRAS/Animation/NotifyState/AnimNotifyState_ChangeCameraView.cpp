// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_ChangeCameraView.h"
#include "Character/Player/RASPlayer.h"

UAnimNotifyState_ChangeCameraView::UAnimNotifyState_ChangeCameraView()
{

}

void UAnimNotifyState_ChangeCameraView::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (ARASPlayer* Player = Cast<ARASPlayer>(Owner))
			{
				Player->SwitchToExecutionCamera(0.2f);
			}
		}
	}

}

void UAnimNotifyState_ChangeCameraView::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (MeshComp)
	{
		if (AActor* Owner = MeshComp->GetOwner())
		{
			if (ARASPlayer* Player = Cast<ARASPlayer>(Owner))
			{
				Player->SwitchBackToOriginalCamera(0.2f);
			}
		}
	}
}
