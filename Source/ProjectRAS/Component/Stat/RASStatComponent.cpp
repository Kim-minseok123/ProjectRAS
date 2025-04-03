// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/RASStatComponent.h"

// Sets default values for this component's properties
URASStatComponent::URASStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float URASStatComponent::ApplyDamage(float InDamageAmount)
{
	const float PrevHp = GetHp();
	const float ActualDamage = FMath::Clamp<float>(InDamageAmount, 0, InDamageAmount);

	SetHp(PrevHp - ActualDamage);
	if (GetHp() <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

float URASStatComponent::ApplyStaminaDamage(float InStaminaDamageAmount)
{
	const float PrevStamina = GetStamina();
	const float ActualStaminaDamage = FMath::Clamp<float>(InStaminaDamageAmount, 0, InStaminaDamageAmount);

	if (ActualStaminaDamage > 0)
		RecoveryDelayRemaining = RecoveryDelayDuration;

	SetStamina(PrevStamina - ActualStaminaDamage);
	if (GetStamina() <= KINDA_SMALL_NUMBER)
	{
		OnStaminaZero.Broadcast(true);
	}

	return ActualStaminaDamage;
}

void URASStatComponent::SetHp(float InHp)
{
	float CurrentHp = FMath::Clamp<float>(InHp, 0.0f, BaseStats.MaxHP);
	
	BaseStats.HP = CurrentHp;

	OnHpChanged.Broadcast(CurrentHp);
}

void URASStatComponent::SetStamina(float InStamina)
{
	float CurrentStamina = FMath::Clamp<float>(InStamina, 0.0f, BaseStats.MaxStamina);

	BaseStats.Stamina = CurrentStamina;

	OnStaminaChanged.Broadcast(CurrentStamina);
}

// Called when the game starts
void URASStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URASStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bRecoveryStamina == false) return;
	if (RecoveryDelayRemaining > 0.f)
	{
		RecoveryDelayRemaining -= DeltaTime;
		return;
	}
	float CurStamina = GetStamina();
	SetStamina(CurStamina + RecoveryRate);
}

