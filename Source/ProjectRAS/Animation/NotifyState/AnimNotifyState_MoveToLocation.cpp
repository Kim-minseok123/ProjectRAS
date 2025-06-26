// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_MoveToLocation.h"
#include "Character/Monster/RASMonster.h"

UAnimNotifyState_MoveToLocation::UAnimNotifyState_MoveToLocation()
{

}

void UAnimNotifyState_MoveToLocation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    FRuntimeData& D = Runtime.FindOrAdd(MeshComp);
    D.StartPos = MeshComp->GetOwner()->GetActorLocation();
    D.Travelled = 0.f;
    D.Elapsed = 0.f;
    D.Duration = TotalDuration;
    if (ARASMonster* Monster = Cast<ARASMonster>(MeshComp->GetOwner()))
    {
        ARASCharacterBase* Target = Monster->GetTarget();
        if (Target)
		{
			D.Target = Target;
		}
		else
		{
			D.Target = nullptr;
		}
    }
}

void UAnimNotifyState_MoveToLocation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    auto* Owner = MeshComp->GetOwner();
    FRuntimeData* D = Runtime.Find(MeshComp);
    if (!Owner || !D) return;

    D->Elapsed += FrameDeltaTime;
    const float TimeLeft = FMath::Max(D->Duration - D->Elapsed, KINDA_SMALL_NUMBER);

    const FVector CurrentTargetPos =
        (D->Target.IsValid() ? D->Target->GetActorLocation()
            : Owner->GetActorLocation());

    FVector Dir = (CurrentTargetPos - Owner->GetActorLocation()).GetSafeNormal();

    const float RemainingDist = FMath::Max(DistanceValue - D->Travelled, 0.f);
    const float Speed = RemainingDist / TimeLeft;
    const FVector MoveDelta = Dir * Speed * FrameDeltaTime;

    Owner->AddActorWorldOffset(MoveDelta, /*bSweep=*/true);
    D->Travelled += MoveDelta.Size();
}

void UAnimNotifyState_MoveToLocation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    FRuntimeData* D = Runtime.Find(MeshComp);
    if (D && D->Travelled < DistanceValue - KINDA_SMALL_NUMBER)
    {
        const float Remainder = DistanceValue - D->Travelled;
        auto* Owner = MeshComp->GetOwner();
        if (Owner && D->Target.IsValid())
        {
            FVector Dir = (D->Target->GetActorLocation() -
                Owner->GetActorLocation()).GetSafeNormal();
            Owner->AddActorWorldOffset(Dir * Remainder, true);
        }
    }
    Runtime.Remove(MeshComp);  
}
