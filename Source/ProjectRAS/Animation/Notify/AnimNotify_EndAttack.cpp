// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_EndAttack.h"
#include "Interface/RASBattleInterface.h"
UAnimNotify_EndAttack::UAnimNotify_EndAttack()
{
}

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(MeshComp->GetOwner());
	if (BattleInterface)
	{
		BattleInterface->EndAttack();
	}
}
