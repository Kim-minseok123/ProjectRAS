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
			MakeLineTrace(MeshComp);
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

void UAnimNotifyState_AttackTo::MakeLineTrace(USkeletalMeshComponent* Attacker)
{
	if (Attacker == nullptr) return;

	const FName StartSocketName(TEXT("WeaponStart"));
	const FName EndSocketName(TEXT("WeaponEnd"));

	if (Attacker->DoesSocketExist(StartSocketName) && Attacker->DoesSocketExist(EndSocketName))
	{
		FVector StartLocation = Attacker->GetSocketLocation(StartSocketName);
		FVector EndLocation = Attacker->GetSocketLocation(EndSocketName);

		AActor* Owner = Attacker->GetOwner();
		if (Owner == nullptr) return;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Owner);

		UWorld* World = Owner->GetWorld();
		if (World == nullptr) return;
		
		bool bHit = World->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_RASChannel,
			CollisionParams
		);
		AActor* HitActor = HitResult.GetActor();

		if (bHit == false || HitActor == nullptr || HitActors.Contains(HitResult.GetActor()))
		{
			DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 0.1f, 0, 1.0f);
			return;
		}
		HitActors.Add(HitActor);

		DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 1.0f);
		DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 1.0f);

		IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(HitActor);
		ARASCharacterBase* From = Cast<ARASCharacterBase>(Owner);
		if (BattleInterface == nullptr)
			return;
		// TODO AttackNum에 따라 데미지 가져오기

		BattleInterface->HitFromActor(From, 0);
	}
}
