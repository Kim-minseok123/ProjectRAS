// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RASGameSingleton.h"
#include "Data/RASCharacterStat.h"
#include "Data/RASDamageData.h"
#include "Data/RASCreatureStatData.h"

URASGameSingleton::URASGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<URASCreatureStatData> StatDataAsset(TEXT("/Script/ProjectRAS.RASCreatureStatData'/Game/1_ProjectRAS/Data/DA_RASCreatureStat.DA_RASCreatureStat'"));
	if (StatDataAsset.Object)
	{
		CharacterStat.Append(StatDataAsset.Object->Stats);
	}
	static ConstructorHelpers::FObjectFinder<URASDamageData> DamageDataAsset(TEXT("/Script/ProjectRAS.RASDamageData'/Game/1_ProjectRAS/Data/DA_RASDamageInfo.DA_RASDamageInfo'"));
	if (DamageDataAsset.Object)
	{
		DamageInfo.Append(DamageDataAsset.Object->Infos);
	}
}

URASGameSingleton& URASGameSingleton::Get()
{
	URASGameSingleton* Singleton = CastChecked<URASGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}
	UE_LOG(LogRASGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<URASGameSingleton>();
}

void URASGameSingleton::SetDeveloperMode(bool bEnable)
{
	bIsDeveloperMode = bEnable;
}

FRASCharacterStats& URASGameSingleton::GetStatForName(const FName InName)
{
	return CharacterStat[InName];
}

FCreatureDamageInfo& URASGameSingleton::GetDamageInfoForName(const FName InName)
{
	return DamageInfo[InName];
}
