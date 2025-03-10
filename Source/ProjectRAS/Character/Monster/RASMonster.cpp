// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/RASMonster.h"
#include "Controller/Monster/RASAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ARASMonster::ARASMonster()
{

}

void ARASMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

ARASCharacterBase* ARASMonster::GetTarget()
{
	return Target;
}

void ARASMonster::SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ARASMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{

}

void ARASMonster::EndAttack()
{
	OnAttackFinished.ExecuteIfBound();
}

void ARASMonster::HitFromActor(class ARASCharacterBase* InFrom, int InDamage)
{
	Super::HitFromActor(InFrom, InDamage);

	ARASMonster* FromMonster = Cast<ARASMonster>(InFrom);
	if (FromMonster != nullptr) return;
}
