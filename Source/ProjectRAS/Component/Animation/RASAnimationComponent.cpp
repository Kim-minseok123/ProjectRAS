// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Animation/RASAnimationComponent.h"

// Sets default values for this component's properties
URASAnimationComponent::URASAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void URASAnimationComponent::StopMontage(UAnimMontage* Montage /*= nullptr*/, float Time /*= 0.f*/)
{
	if (MyAnimInstance)
	{
		if (Montage == nullptr)
		{
			MyAnimInstance->Montage_Stop(Time);
		}
		else
		{
			MyAnimInstance->Montage_Stop(Time, Montage);
		}
	}
}

void URASAnimationComponent::PlayMontageWithSection(UAnimMontage* Montage, FName SectionName, float PlayRate, TFunction<void(UAnimMontage*, bool)> MontageEndedCallback)
{
	if (!MyAnimInstance || !Montage)
	{
		return;
	}

	MyAnimInstance->Montage_Play(Montage, PlayRate);
	MyAnimInstance->Montage_JumpToSection(SectionName, Montage);

	if(MontageEndedCallback == nullptr)
	{
		return;
	}

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([MontageEndedCallback](UAnimMontage* EndedMontage, bool bInterrupted)
		{
			MontageEndedCallback(EndedMontage, bInterrupted);
		});
	MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
}

TObjectPtr<class UAnimMontage> URASAnimationComponent::GetMontageByName(FName MontageName) const
{
	if(MontageName == "Hit")
	{
		return HitMontage;
	}
	if(MontageName == "Death")
	{
		return DeathMontage;
	}
	return nullptr;
}

void URASAnimationComponent::ClearAllDelegate()
{
	if (MyAnimInstance)
	{
		MyAnimInstance->OnMontageEnded.RemoveAll(GetOwner());
	}
}

void URASAnimationComponent::ChangeRootMotionMode(bool bEnable)
{
	if (MyAnimInstance)
	{
		if (bEnable)
		{
			MyAnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		}
		else
		{
			MyAnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}
	}
}

void URASAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void URASAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

