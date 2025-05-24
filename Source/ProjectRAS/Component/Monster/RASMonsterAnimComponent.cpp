// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Monster/RASMonsterAnimComponent.h"

URASMonsterAnimComponent::URASMonsterAnimComponent()
{

}

TObjectPtr<class UAnimMontage> URASMonsterAnimComponent::GetMontageByName(FName MontageName) const
{
	UAnimMontage* Montage = Super::GetMontageByName(MontageName);
	if (Montage)
	{
		return Montage;
	}
	if (MontageName == "Attack")
	{
		return AttackMontage;
	}
	else if (MontageName == "Normal")
	{
		return NormalAttackMontage;
	}
	else if (MontageName == "Combo")
	{
		return ComboMontage;
	}
	else if (MontageName == "Start")
	{
		return StartMontage;
	}
	return nullptr;
}
