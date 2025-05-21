// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_SpawnItem.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

UAnimNotifyState_SpawnItem::UAnimNotifyState_SpawnItem()
{

}

void UAnimNotifyState_SpawnItem::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		FVector SpawnLocation = MeshComp->GetSocketLocation(SocketName);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = MeshComp->GetOwner();
		SpawnParams.Instigator = Cast<APawn>(MeshComp->GetOwner());
		UWorld* World = MeshComp ? MeshComp->GetWorld() : nullptr;
		if (!World) return;
		
		AActor* NewActor = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewActor)
		{
			NewActor->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			ItemActor = NewActor;
		}
	}
}

void UAnimNotifyState_SpawnItem::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (ItemActor)
	{
		ItemActor->Destroy();
		ItemActor = nullptr;
	}
}
