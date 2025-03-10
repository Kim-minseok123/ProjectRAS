// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Common/RASCommonMonster.h"
#include "Controller/Monster/Common/RASAICommonController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/Stat/RASStatComponent.h"

ARASCommonMonster::ARASCommonMonster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Meshes/Khaimera.Khaimera'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/1_ProjectRAS/Animation/Monster/Common/BP_RASCommonMonsterAnimation.BP_RASCommonMonsterAnimation_C'"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	AIControllerClass = ARASAICommonController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CreatureName = TEXT("Hector");
}

void ARASCommonMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARASCommonMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float PlayRate = FMath::FRandRange(1.0f, 1.5f);
		AnimInstance->Montage_Play(AttackMontage, PlayRate);
		AnimInstance->Montage_JumpToSection(TEXT("Attack"));

		bUnflinching = true;
	}
}

void ARASCommonMonster::EndAttack()
{
	Super::EndAttack();

	bUnflinching = false;

}

void ARASCommonMonster::HitFromActor(class ARASCharacterBase* InFrom, int InDamage)
{
	Super::HitFromActor(InFrom, InDamage);

	Target = InFrom;

	if (float ActualDamage = Stat->ApplyDamage(InDamage) > 0)
	{
		if (bUnflinching == false)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance == nullptr)
				return;

			AnimInstance->Montage_Play(HitMontage);
			if (ActualDamage >= KnockbackFigure)
			{
				AnimInstance->Montage_JumpToSection(TEXT("Knockback"));
			}
			else
			{
				AnimInstance->Montage_JumpToSection(TEXT("Hit"));
			}
		}
	}
}
