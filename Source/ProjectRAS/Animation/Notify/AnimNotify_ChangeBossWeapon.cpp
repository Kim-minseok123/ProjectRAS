// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_ChangeBossWeapon.h"
#include "Character/Monster/Boss/RASBossMonster.h"
#include "Animation/Monster/Boss/RASBossAnimInstance.h"

UAnimNotify_ChangeBossWeapon::UAnimNotify_ChangeBossWeapon()
{
	bWeaponOn = false;
}

void UAnimNotify_ChangeBossWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	URASBossAnimInstance* AnimInstance = Cast<URASBossAnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetHideWeapon(bWeaponOn);
		ARASBossMonster* Boss = Cast<ARASBossMonster>(AnimInstance->GetOwningActor());
		if (Boss)
		{
			Boss->SetWeaponOn(bWeaponOn);
		}
		
	}
}
