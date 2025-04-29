// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_AttackTo.h"
#include "Interface/RASBattleInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/RASCharacterBase.h"
#include "Utils/RASCollisionChannels.h"

UAnimNotifyState_AttackTo::UAnimNotifyState_AttackTo()
{

}

void UAnimNotifyState_AttackTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	HitActors.Empty();
}

void UAnimNotifyState_AttackTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ARASCharacterBase* Attacker = Cast<ARASCharacterBase>(Owner))
		{
			MakeSweepTrace(MeshComp);
		}
	}
}

void UAnimNotifyState_AttackTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

void UAnimNotifyState_AttackTo::MakeSweepTrace(USkeletalMeshComponent* Attacker)
{
	if (!Attacker || !Attacker->DoesSocketExist(StartSocketName) || !Attacker->DoesSocketExist(EndSocketName))
		return;

	const FVector Start = Attacker->GetSocketLocation(StartSocketName);
	const FVector End = Attacker->GetSocketLocation(EndSocketName);

	AActor* Owner = Attacker->GetOwner();
	if (!Owner) return;

	UWorld* World = Owner->GetWorld();
	if (!World) return;

	// ───────────── Sphere Sweep ─────────────
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AttackSweep), false, Owner);
	Params.bReturnPhysicalMaterial = false;

	const FCollisionShape Shape = FCollisionShape::MakeSphere(TraceRadius);
	TArray<FHitResult> HitResults;

	const bool bHit = World->SweepMultiByChannel(
		HitResults, Start, End, FQuat::Identity, ECC_RASChannel, Shape, Params);

#if !(UE_BUILD_SHIPPING)
	// 디버그 시각화
	const FVector Mid = (Start + End) * 0.5f;
	const FQuat Rot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();
	DrawDebugCapsule(World, Mid, (End - Start).Size() * 0.5f, TraceRadius, Rot,
		bHit ? FColor::Red : FColor::Blue, false, 0.15f);
#endif

	if (!bHit) return;

	// ───────────── 데미지 처리 ─────────────
	for (const FHitResult& Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActors.Contains(HitActor) || HitActor == Owner) continue;
			HitActors.Add(HitActor);

			if (IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(HitActor))
			{
				auto* From = Cast<ARASCharacterBase>(Owner);
				if (!From) continue;

				const float Damage = From->GetDamageOfAttackNumber(AttackNum);
				const float StaminaDamage = From->GetStaminaDamageOfAttackNumber(AttackNum);
				BattleInterface->HitFromActor(From, Damage, StaminaDamage);
			}
		}
	}
}
