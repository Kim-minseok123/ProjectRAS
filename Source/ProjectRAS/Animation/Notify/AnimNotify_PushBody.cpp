// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_PushBody.h"
#include "Utils/RASCollisionChannels.h"
#include "Engine/OverlapResult.h"
#include "Interface/RASBattleInterface.h"
#include "Character/RASCharacterBase.h"

UAnimNotify_PushBody::UAnimNotify_PushBody()
{
}

void UAnimNotify_PushBody::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* MyActor = MeshComp->GetOwner();

    FVector Center = MyActor->GetActorLocation() + (MyActor->GetActorForwardVector() * 100.f);

    float Width = 100.f;
    float Height = 100.f;
    float Depth = 100.f;

    FVector HalfExtents(Depth * 0.5f, Width * 0.5f, Height * 0.5f);

    FRotator BoxRotation = MyActor->GetActorRotation();
    FQuat BoxOrientation = BoxRotation.Quaternion();

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(Knockback), false, MyActor);

    DrawDebugBox(
        MyActor->GetWorld(),
        Center,
        HalfExtents,
        BoxOrientation,
        FColor::Red,
        false, // 지속 시간 (true면 영구 표시)
        2.0f   // 2초 동안 표시
    );

    bool bIsHit = MyActor->GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        Center,
        BoxOrientation,
        ECC_RASChannel,
        FCollisionShape::MakeBox(HalfExtents),
        CollisionParams
    );

    if (bIsHit)
    {
        for (auto& Overlap : OverlapResults)
        {
            AActor* HitActor = Overlap.GetActor();
            IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(HitActor);
            if (BattleInterface)
            {
                BattleInterface->HitFromActor(Cast<ARASCharacterBase>(MyActor), 15.f);
                FVector Direction = HitActor->GetActorLocation() - Center;
                BattleInterface->KnockbackToDirection(MyActor, Direction, 1300.f);
            }
        }
    }
}
