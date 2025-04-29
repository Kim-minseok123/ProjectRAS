// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_CircularRangeAttack.h"

UAnimNotify_CircularRangeAttack::UAnimNotify_CircularRangeAttack()
{

}

void UAnimNotify_CircularRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
}
