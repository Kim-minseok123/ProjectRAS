// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_AttackToThrow.h"
#include "Interface/RASBattleInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/RASCharacterBase.h"
#include "Utils/RASCollisionChannels.h"
#include "Interface/Monster/RASMonsterInfoInterface.h"
#include "Character/Projectile/RASProjectile.h"

UAnimNotifyState_AttackToThrow::UAnimNotifyState_AttackToThrow()
{

}

void UAnimNotifyState_AttackToThrow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && Projectile)
	{
        ARASCharacterBase* ControllingActor = Cast<ARASCharacterBase>(MeshComp->GetOwner());
        if (ControllingActor == nullptr) return;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = ControllingActor;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = ControllingActor->GetActorLocation() + LocationOffset;
        FRotator SpawnRotation = ControllingActor->GetActorRotation() + RotationOffset;

        AActor* SpawnedActor = ControllingActor->GetWorld()->SpawnActor<AActor>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);

        if (SpawnedActor)
        {
            ARASProjectile* SpawnProjectile = Cast<ARASProjectile>(SpawnedActor);
            float AcutalDamage = ControllingActor->GetDamageOfAttackNumber(AttackNum);
            float AcutalStaminaDamage = ControllingActor->GetStaminaDamageOfAttackNumber(AttackNum);
            
            FVector TargetLocation = Cast<IRASMonsterInfoInterface>(ControllingActor)->GetTarget()->GetActorLocation() - SpawnLocation;
            if (SpawnProjectile) SpawnProjectile->FireProjectile(ControllingActor, TargetLocation, AcutalDamage, AcutalStaminaDamage);
        }
	}
}

void UAnimNotifyState_AttackToThrow::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_AttackToThrow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

    if (MeshComp)
    {
        IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(MeshComp->GetOwner());
        if (BattleInterface)
        {
            BattleInterface->EndAttack();
        }
    }
}
