// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASPlayerAnimComponent.h"

URASPlayerAnimComponent::URASPlayerAnimComponent()
{

}

TObjectPtr<class UAnimMontage> URASPlayerAnimComponent::GetMontageByName(FName MontageName) const
{
	UAnimMontage* Montage = Super::GetMontageByName(MontageName);
	if (Montage)
	{
		return Montage;
	}

	if(MontageName == "Skill")
	{
		return SkillMontage;
	}
	else if (MontageName == "Parrying")
	{
		return ParryingMontage;
	}
	else if (MontageName == "Execute")
	{
		return ExecuteMontage;
	}
	else if (MontageName == "Roll")
	{
		return RollMontage;
	}
	else if (MontageName == "Potion")
	{
		return PotionMontage;
	}
	return nullptr;
}

bool URASPlayerAnimComponent::StopParryingAnimation()
{
	UAnimMontage* CurrentMontage = MyAnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		FName CurrentSection = MyAnimInstance->Montage_GetCurrentSection(CurrentMontage);
		if (CurrentSection != TEXT("ParryingBreak") && CurrentSection != TEXT("ParryingExact"))
		{
			MyAnimInstance->Montage_Stop(0.25f, ParryingMontage);
			return true;
		}
	}
	return false;
}

void URASPlayerAnimComponent::ClearAllDelegate()
{
	if (MyAnimInstance)
	{
		FOnMontageEnded DummyDelegate;
		if (HitMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, HitMontage);
		}
		if (SkillMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, SkillMontage);
		}
		if (ParryingMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, ParryingMontage);
		}
		if (ExecuteMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, ExecuteMontage);
		}
		if (RollMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, RollMontage);
		}
		if (PotionMontage)
		{
			MyAnimInstance->Montage_SetEndDelegate(DummyDelegate, PotionMontage);
		}
	}
}
