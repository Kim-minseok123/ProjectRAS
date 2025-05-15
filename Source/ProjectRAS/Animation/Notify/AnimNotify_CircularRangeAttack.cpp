// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_CircularRangeAttack.h"
#include "Character/Monster/Boss/RASBossMonster.h"
#include "Interface/RASBattleInterface.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Character/RASCharacterBase.h"
#include "Utils/RASCollisionChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

UAnimNotify_CircularRangeAttack::UAnimNotify_CircularRangeAttack()
{
	bPlayParticle = true;
}

void UAnimNotify_CircularRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (ARASBossMonster* Boss = Cast<ARASBossMonster>(MeshComp->GetOwner()))
	{
		FVector Center = Boss->GetCircleAttackPosition();

		const FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
		TArray<FHitResult> HitResults;

		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, Boss);
		Params.bReturnPhysicalMaterial = false;

		if (bPlayParticle && Particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				Boss->GetWorld(),
				Particle,
				Center,
				FRotator::ZeroRotator,
				true
			);
		}
		bool bHit = Boss->GetWorld()->SweepMultiByChannel(
			HitResults,
			Center,
			Center,
			FQuat::Identity,
			ECC_RASChannel,
			Shape,
			Params
		);
#if !(UE_BUILD_SHIPPING)
		// 디버그 시각화
		DrawDebugSphere(Boss->GetWorld(), Center, Radius, 16,
			bHit ? FColor::Red : FColor::Blue, false, 0.15f);
#endif
		if(!bHit) return;
		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor == Boss) continue;

				if (IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(HitActor))
				{
					auto* From = Cast<ARASCharacterBase>(Boss);
					if (!From) continue;

					const float Damage = From->GetDamageOfAttackNumber(AttackNum);
					const float StaminaDamage = From->GetStaminaDamageOfAttackNumber(AttackNum);
					BattleInterface->HitFromActor(From, Damage, StaminaDamage);
				}
			}
		}
	}
}
