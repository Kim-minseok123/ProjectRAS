// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_SetExecuteLocation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UAnimNotify_SetExecuteLocation::UAnimNotify_SetExecuteLocation()
{

}

void UAnimNotify_SetExecuteLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    if (!MeshComp)
    {
        return;
    }

    AActor* MyActor = MeshComp->GetOwner();
    UWorld* World = MeshComp->GetWorld();
    if (!World)
    {
        return;
    }

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (MyActor && PlayerCharacter)
    {
        FVector NewLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * Offset;
        MyActor->SetActorLocation(NewLocation);
    }
}

