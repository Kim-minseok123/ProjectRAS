// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_DeathEvent.h"
#include "Character/RASCharacterBase.h"
#include "Components/CapsuleComponent.h"

UAnimNotify_DeathEvent::UAnimNotify_DeathEvent()
{

}

void UAnimNotify_DeathEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ChangeDeathCollision(MeshComp);
}

void UAnimNotify_DeathEvent::ChangeDeathCollision(USkeletalMeshComponent* MeshComp)
{
	if(MeshComp == nullptr) return;
	
	ARASCharacterBase* Character = Cast<ARASCharacterBase>(MeshComp->GetOwner());
	if(Character == nullptr) return;
	Character->GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCollision"));
}
