// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_MoveLocation.h"

UAnimNotifyState_MoveLocation::UAnimNotifyState_MoveLocation()
{

}

void UAnimNotifyState_MoveLocation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AActor* MyActor = MeshComp->GetOwner();
	if (MyActor == nullptr) return;

	FVector CurrentLocation = MyActor->GetActorLocation();
	FVector Offset = MyActor->GetActorForwardVector() * MoveValue;
	FVector NewLocation = CurrentLocation - Offset; 
	MyActor->SetActorLocation(NewLocation);

}
