// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_BossAttackTo.h"
#include "Interface/RASBattleInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/RASCharacterBase.h"
#include "Utils/RASCollisionChannels.h"
#include "Character/Monster/Boss/RASBossMonster.h"

UAnimNotifyState_BossAttackTo::UAnimNotifyState_BossAttackTo()
{

}

void UAnimNotifyState_BossAttackTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	HitActors.Empty();
}

void UAnimNotifyState_BossAttackTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (auto* Boss = Cast<ARASBossMonster>(MeshComp->GetOwner()))
	{
		MakeSweepTrace(Boss);
	}
}

void UAnimNotifyState_BossAttackTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{	
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp)
	{
		IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(MeshComp->GetOwner());
		if (BattleInterface)
		{
			//BattleInterface->EndAttack();
		}
	}

}

void UAnimNotifyState_BossAttackTo::MakeSweepTrace(ARASBossMonster* Boss)
{
	if (!Boss) return;

	const TArray<FVector> WeaponLocs = Boss->GetWeaponPosition();
	if (WeaponLocs.Num() != 2) return;

	const FVector Start = WeaponLocs[0];
	const FVector End = WeaponLocs[1];

	UWorld* World = Boss->GetWorld();
	if (!World) return;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(BossAttackSweep), false, Boss);
	const FCollisionShape Shape = FCollisionShape::MakeSphere(TraceRadius);

	TArray<FHitResult> Hits;
	const bool bHit = World->SweepMultiByChannel(
		Hits, Start, End, FQuat::Identity, ECC_RASChannel, Shape, Params);

#if !(UE_BUILD_SHIPPING)
	// 디버그 시각화
	const FVector Mid = (Start + End) * 0.5f;
	const FQuat Rot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
	DrawDebugCapsule(World, Mid, (End - Start).Size() * 0.5f, TraceRadius, Rot,
		bHit ? FColor::Red : FColor::Blue, false, 0.15f);
#endif

	if (!bHit) return;

	for (const FHitResult& Hit : Hits)
	{
		if (AActor* Victim = Hit.GetActor())
		{
			if (Victim == Boss || HitActors.Contains(Victim)) continue;
			HitActors.Add(Victim);

			if (IRASBattleInterface* BI = Cast<IRASBattleInterface>(Victim))
			{
				const float Dmg = Boss->GetDamageOfAttackNumber(AttackNum);
				const float StmDmg = Boss->GetStaminaDamageOfAttackNumber(AttackNum);
				BI->HitFromActor(Boss, Dmg, StmDmg);
			}
		}
	}
}
