// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/RASMonster.h"
#include "Controller/Monster/RASAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/RASPlayer.h"

ARASMonster::ARASMonster()
{

	Indicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Indicator"));
	Indicator->SetupAttachment(RootComponent);
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

	ARASPlayer* FromPlayer = Cast<ARASPlayer>(InFrom);
	if (FromPlayer == nullptr) return;
	FromPlayer->SetLockedOnTarget(this);
}

void ARASMonster::SetVisibleIndicator(bool InbIsVisible)
{
	if (Indicator == nullptr) return;
	Indicator->SetVisibility(InbIsVisible);
}
