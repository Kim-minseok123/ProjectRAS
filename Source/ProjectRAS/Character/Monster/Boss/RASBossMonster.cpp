// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Boss/RASBossMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/Stat/RASStatComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Components/WidgetComponent.h"
#include "UI/RASStatusBarWidget.h"
#include "Data/RASGameSingleton.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/RASAimWidget.h"
#include "Component/Player/RASCombatComponent.h"
#include "Component/Monster/RASMonsterAnimComponent.h"
#include "Controller/Monster/Boss/RASAIBossController.h"

ARASBossMonster::ARASBossMonster()
{
	AIControllerClass = ARASAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CreatureName = TEXT("Ashur");
}

void ARASBossMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsDeath) return;
	if (Target && bUnflinching)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		SetActorRotation(FQuat::FindBetweenVectors(GetActorForwardVector(), Direction));
	}
}

void ARASBossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->BaseStats = URASGameSingleton::Get().GetStatForName(CreatureName);
	CreatureDamageInfo = URASGameSingleton::Get().GetDamageInfoForName(CreatureName);

	// Boss ui 연동
}

void ARASBossMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		FString AttackSectionName = FString::Printf(TEXT("Attack%d"), InAttackNumber);
		UE_LOG(LogTemp, Log, TEXT("%s"), *AttackSectionName);
		MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Attack")), *AttackSectionName, 1.f);

		bUnflinching = true;
	}
}

void ARASBossMonster::EndAttack()
{
	Super::EndAttack();

	bUnflinching = false;
}

void ARASBossMonster::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	ARASPlayer* FromPlayer = Cast<ARASPlayer>(InFrom);
	if (FromPlayer == nullptr) return;

	Target = InFrom;

	if (float ActualDamage = Stat->ApplyDamage(InDamage) > 0)
	{
		FRotator LookAtFrom = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InFrom->GetActorLocation());
		SetActorRotation(LookAtFrom);
		Stat->ApplyStaminaDamage(InStaminaDamage);
		if (Stat->GetHp() <= 0) return;
		
		if (bUnflinching == false)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Hit")), TEXT("Hit"), 1.0f);
		}
	}
}

void ARASBossMonster::KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower)
{
	Super::KnockbackToDirection(InFrom, Direction, InPower);

	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Hit")), TEXT("Knockback"), 1.0f);
			AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}

		float KnockbackStrength = InPower;
		LaunchCharacter(Direction * KnockbackStrength, true, true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AnimInstance]()
			{
				if (AnimInstance)
				{
					AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
				}
			}, 0.2f, false);
	}

	if (OnStopAttack.IsBound() == true)
	{
		OnStopAttack.Broadcast();
	}

	EndAttack();
}

void ARASBossMonster::Death()
{
	
}

void ARASBossMonster::ExecuteDeath(int32 InDeathNumber)
{

}

void ARASBossMonster::Dash(FVector InDireciton, float InDashSpeed)
{

}

void ARASBossMonster::PreDeath()
{
	ARASAIBossController* MyController = Cast<ARASAIBossController>(GetController());

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCollision"));

	MyController->StopAI();
	IndicatorWideget->SetVisibility(false);

	bIsDeath = true;

	MonsterAnimComponent->StopMontage(nullptr, 0.1f);
	MonsterAnimComponent->ClearAllDelegate();
	MonsterAnimComponent->ChangeRootMotionMode(true);
}
