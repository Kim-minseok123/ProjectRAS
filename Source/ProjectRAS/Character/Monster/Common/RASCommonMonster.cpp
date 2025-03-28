// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Common/RASCommonMonster.h"
#include "Controller/Monster/Common/RASAICommonController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/Stat/RASStatComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Components/WidgetComponent.h"
#include "UI/RASStatusBarWidget.h"
#include "Data/RASGameSingleton.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/RASAimWidget.h"

ARASCommonMonster::ARASCommonMonster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Meshes/Khaimera.Khaimera'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/1_ProjectRAS/Animation/Monster/Common/BP_RASCommonMonsterAnimation.BP_RASCommonMonsterAnimation_C'"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

	StatusBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusBar"));
	StatusBarWidgetComponent->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UUserWidget> StatusBarWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_StatusBar.WBP_StatusBar_C'"));
	if(StatusBarWidgetRef.Class)
	{
		StatusBarWidgetComponent->SetWidgetClass(StatusBarWidgetRef.Class);
		StatusBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		StatusBarWidgetComponent->SetDrawSize(FVector2D(175, 50));
		StatusBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 150));
	}

	AIControllerClass = ARASAICommonController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CreatureName = TEXT("Hector");

	StatusBarWidgetComponent->SetVisibility(false);
}

void ARASCommonMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Target && bUnflinching && bIsDeath == false)
	{
		FVector MyLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();

		FRotator CurrentRotation = GetActorRotation();
		FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, DesiredRotation.Yaw, CurrentRotation.Roll), DeltaSeconds, 100.f);

		SetActorRotation(NewRotation);
	}
}

void ARASCommonMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->BaseStats = URASGameSingleton::Get().GetStatForName(CreatureName);
	CreatureDamageInfo = URASGameSingleton::Get().GetDamageInfoForName(CreatureName);

	StatusBarWidgetComponent->InitWidget();

	auto Widget = Cast<URASStatusBarWidget>(StatusBarWidgetComponent->GetUserWidgetObject());
	if (Widget == nullptr) return;

	Widget->BindHP(Stat);
	Widget->BindStamina(Stat);

	Stat->SetHp(100000);
	Stat->SetStamina(100000);

	Stat->OnHpZero.AddUObject(this, &ARASCommonMonster::Death);
}

void ARASCommonMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		float PlayRate = FMath::FRandRange(1.0f, 1.5f);
		AnimInstance->Montage_Play(AttackMontage, PlayRate);
		AnimInstance->Montage_JumpToSection(TEXT("Attack"));

		bUnflinching = true;
	}
}

void ARASCommonMonster::EndAttack()
{
	Super::EndAttack();

	bUnflinching = false;

}

void ARASCommonMonster::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	ARASPlayer* FromPlayer = Cast<ARASPlayer>(InFrom);
	if (FromPlayer == nullptr) return;

	Target = InFrom;

	if (float ActualDamage = Stat->ApplyDamage(InDamage) > 0 )
	{
		Stat->ApplyStaminaDamage(InStaminaDamage);
		StatusBarWidgetComponent->SetVisibility(true);
		if (bUnflinching == false)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance == nullptr)
				return;

			AnimInstance->Montage_Play(HitMontage);
			if (ActualDamage >= KnockbackFigure)
			{
				AnimInstance->Montage_JumpToSection(TEXT("Knockback"));
			}
			else
			{
				AnimInstance->Montage_JumpToSection(TEXT("Hit"));
			}
		}
	}
}

void ARASCommonMonster::KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower)
{
	Super::KnockbackToDirection(InFrom, Direction, InPower);

	if(OnStopAttack.IsBound() == true) 	
	{
		OnStopAttack.Broadcast();
	}

	EndAttack();
}

void ARASCommonMonster::Death()
{
	ARASAICommonController* MyController = Cast<ARASAICommonController>(GetController());
	MyController->StopAI();
	IndicatorWideget->SetVisibility(false);
	StatusBarWidgetComponent->SetVisibility(false);
	bIsDeath = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCollision"));
	
	ARASPlayer* Player = Cast<ARASPlayer>(Target);
	if (Player != nullptr) 
		Player->PressTab();

	Super::Death();

	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			Destroy();
		},
		5.f, false);
}
