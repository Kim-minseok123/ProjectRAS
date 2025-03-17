// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_KnockBackTrigger.h"
#include "Interface/RASBattleInterface.h"
#include "Utils/RASCollisionChannels.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UAnimNotify_KnockBackTrigger::UAnimNotify_KnockBackTrigger()
{

}

void UAnimNotify_KnockBackTrigger::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* MyActor = MeshComp->GetOwner();

	FVector Center = MyActor->GetActorLocation();
	const float SphereRadius = 500.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(KnockBack), false, MyActor);
	bool bResult = MyActor->GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_RASChannel,
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionQueryParam
	);
	DrawDebugSphere(MyActor->GetWorld(), Center, SphereRadius, 16, FColor::Green, false, 0.2f);

	if (bResult)
	{
		for (auto& Overlap : OverlapResults)
		{
			FVector TargetLocation = Overlap.GetActor()->GetActorLocation();
			IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(Overlap.GetActor());
			if (BattleInterface)
			{
				FVector Direction = TargetLocation - Center;
				BattleInterface->KnockbackToDirection(MyActor, Direction, 1800.f);
			}
		}
	}
}
