// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NotifyState/AnimNotifyState_AttackTo.h"
#include "Interface/RASBattleInterface.h"
UAnimNotifyState_AttackTo::UAnimNotifyState_AttackTo()
{

}

void UAnimNotifyState_AttackTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (MeshComp)
	{
		IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(MeshComp->GetOwner());
		if (BattleInterface)
		{
			BattleInterface->AttackTo(AttackNum);
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
