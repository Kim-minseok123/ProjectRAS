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
