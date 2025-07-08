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
#include "Component/Player/RASCombatComponent.h"
#include "Component/Monster/RASMonsterAnimComponent.h"

ARASCommonMonster::ARASCommonMonster()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKhaimera/Characters/Heroes/Khaimera/Meshes/Khaimera.Khaimera'"));
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/1_ProjectRAS/Animation/Monster/Common/Hector/BP_RASCommonMonsterAnimation.BP_RASCommonMonsterAnimation_C'"));
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
	if (bIsDeath) return;
	if (Target)
	{
		if (ARASPlayer* Player = Cast<ARASPlayer>(Target))
		{
			if (Player->GetCombatComponent()->GetCombatState() == EPlayerCombatState::Deathing)
			{
				ARASAICommonController* MyController = Cast<ARASAICommonController>(GetController());
				if (MyController)
				{
					MyController->ClearTarget();
				}
				Target = nullptr;
				return;
			}
		}
	}
	if (Target && bUnflinching)
	{
		FVector MyLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();

		FRotator CurrentRotation = GetActorRotation();
		FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, DesiredRotation.Yaw, CurrentRotation.Roll), DeltaSeconds, 100.f);

		SetActorRotation(NewRotation);
	}
	if (Target && bIsDashing)
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
		float PlayRate = FMath::FRandRange(1.15f, 1.2f);

		FString AttackSectionName = FString::Printf(TEXT("Attack%d"), InAttackNumber);
		UE_LOG(LogTemp, Log, TEXT("%s"), *AttackSectionName);
		MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Attack")), *AttackSectionName, PlayRate);

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
		FRotator LookAtFrom = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InFrom->GetActorLocation());
		SetActorRotation(LookAtFrom);
		Stat->ApplyStaminaDamage(InStaminaDamage);
		if (Stat->GetHp() <= 0) return;
		StatusBarWidgetComponent->SetVisibility(true);
		if (bUnflinching == false)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Hit")), TEXT("Hit"), 1.0f);
		}
	}
}

void ARASCommonMonster::KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower)
{
	Super::KnockbackToDirection(InFrom, Direction, InPower);

	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Hit")), TEXT("Knockback"), 1.0f);
			AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
		}

		float KnockbackStrength = InPower;
		LaunchCharacter(Direction * KnockbackStrength, true, true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, AnimInstance]()
			{
				if (AnimInstance)
				{
					AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
				}
			}, 0.2f, false);
	}

	if(OnStopAttack.IsBound() == true) 	
	{
		OnStopAttack.Broadcast();
	}

	EndAttack();
}

void ARASCommonMonster::PreDeath()
{
	ARASAICommonController* MyController = Cast<ARASAICommonController>(GetController());

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCollision"));

	MyController->StopAI();
	IndicatorWideget->SetVisibility(false);
	auto Widget = Cast<URASStatusBarWidget>(StatusBarWidgetComponent->GetUserWidgetObject());
	if (Widget == nullptr) return;
	Widget->SetVisibilityBar(false);
	bIsDeath = true;

	MonsterAnimComponent->StopMontage(nullptr, 0.1f);
	MonsterAnimComponent->ClearAllDelegate();
	MonsterAnimComponent->ChangeRootMotionMode(true);
}

void ARASCommonMonster::Death()
{
	PreDeath();

	int DeathType = FMath::RandRange(0, 1);


	if (DeathType == 0)
	{
		MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Death")), TEXT("FrontDeath"), 1.f);
	}
	else
	{
		MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Death")), TEXT("BackDeath"), 1.f);
	}
	
	ARASPlayer* Player = Cast<ARASPlayer>(Target);
	if (Player != nullptr) 
		Player->GetCombatComponent()->PressTab();
	OnRoomClear.ExecuteIfBound();


	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			Destroy();
		},
		5.f, false);
}

void ARASCommonMonster::ExecuteDeath(int32 InDeathNumber)
{
	PreDeath();

	FString MontageSectionName = FString::Printf(TEXT("ExecuteDeath%d"), InDeathNumber);

	MonsterAnimComponent->PlayMontageWithSection(
		MonsterAnimComponent->GetMontageByName(TEXT("Death")), FName(*MontageSectionName), 1.f);
	OnRoomClear.ExecuteIfBound();

	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			Destroy();
		},
		5.f, false);
}

void ARASCommonMonster::Dash(FVector InDireciton, float InDashSpeed)
{
	InDireciton.Normalize();

	bIsDashing = true;

	FVector DashVelocity = InDireciton * InDashSpeed;  

	float ArcHeight = 300.0f; 
	DashVelocity.Z += ArcHeight;  

	LaunchCharacter(DashVelocity, true, true); 

	FTimerHandle Handler;
	GetWorld()->GetTimerManager().SetTimer(Handler, [this]()
		{
			bIsDashing = false;
		}, 1.f, false);
	
}
