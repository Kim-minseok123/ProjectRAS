// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Common/RASCommonMonster.h"
#include "Controller/Monster/Common/RASAICommonController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

ARASCommonMonster::ARASCommonMonster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Meshes/Khaimera.Khaimera'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Khaimera_AnimBlueprint.Khaimera_AnimBlueprint_C'"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	AIControllerClass = ARASAICommonController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ARASCommonMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
