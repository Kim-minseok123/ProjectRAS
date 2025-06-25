// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/Boss/RASBossMonster.h"
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
#include "Controller/Monster/Boss/RASAIBossController.h"
#include "Data/RASBossScoreData.h"
#include "Utils/RASBlackBoardKey.h"
#include "UI/RASBossHUDWidget.h"
#include "Audio/RASAudioSubsystem.h"

ARASBossMonster::ARASBossMonster()
{
	AIControllerClass = ARASAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CreatureName = TEXT("Ashur");

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName(TEXT("hand_l")));
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));


	WeaponStart = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponStart"));
	WeaponStart->SetupAttachment(WeaponMesh, TEXT("WeaponStart"));

	WeaponEnd = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponEnd"));
	WeaponEnd->SetupAttachment(WeaponMesh, TEXT("WeaponEnd"));

	WeaponCircleAttack = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponCircleAttack"));
	WeaponCircleAttack->SetupAttachment(WeaponMesh, TEXT("WeaponCircleAttack"));

	static ConstructorHelpers::FClassFinder<URASBossHUDWidget> BossHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_BossHUD.WBP_BossHUD_C'"));
	if (BossHUDWidgetRef.Class)
	{
		BossHUDWidgetClass = BossHUDWidgetRef.Class;
	}
}

void ARASBossMonster::BeginPlay()
{
	Super::BeginPlay();
	Target = Cast<ARASCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Target)
	{
		UE_LOG(LogTemp, Log, TEXT("Target : %s"), *Target->GetName());
		ARASAIBossController* MyController = Cast<ARASAIBossController>(GetController());
		if (MyController)
		{
			MyController->SetTargetBlackboard();
		}
	}
	SkillScoreDataMap = BossScoreData->SkillScoreDataMap;

	for (auto& Skill : SkillScoreDataMap)
	{
		Skill.Value.LastUsedTime = -Skill.Value.Cooldown;
	}

	GetGameInstance()->GetSubsystem<URASAudioSubsystem>()->PlayBGM(TEXT("Boss"));
}

void ARASBossMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsDeath) return;
	if (Target)
	{
		FVector MyLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();

		FRotator CurrentRotation = GetActorRotation();
		FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(CurrentRotation.Pitch, DesiredRotation.Yaw, CurrentRotation.Roll), DeltaSeconds, 100.f);

		SetActorRotation(NewRotation);
	}
}

void ARASBossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->BaseStats = URASGameSingleton::Get().GetStatForName(CreatureName);
	CreatureDamageInfo = URASGameSingleton::Get().GetDamageInfoForName(CreatureName);

	InitUI();

	Stat->SetHp(100000);
	Stat->SetStamina(100000);

	Stat->OnHpZero.AddUObject(this, &ARASBossMonster::Death);
	// Boss ui 연동
}

void ARASBossMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		FString AttackSectionName = FString::Printf(TEXT("Attack%d"), InAttackNumber);
		UE_LOG(LogTemp, Log, TEXT("%s"), *AttackSectionName);

		if (InAttackNumber == 1)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Normal")), *AttackSectionName, 1.f,
				[this, InAttackNumber](UAnimMontage* Montage, bool bInterrupted)
				{
					SkillScoreDataMap[InAttackNumber].LastUsedTime = GetWorld()->GetTimeSeconds();
					Cast<ARASAIBossController>(GetController())->GetBlackboardComponent()->SetValueAsInt(BBBestSkillIndex, -1);
					UE_LOG(LogTemp, Log, TEXT("쿨타임 기록됨q"));
				});
		}
		else if (InAttackNumber <= 3)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Attack")), *AttackSectionName, 1.f,
				[this, InAttackNumber](UAnimMontage* Montage, bool bInterrupted)
				{
					SkillScoreDataMap[InAttackNumber].LastUsedTime = GetWorld()->GetTimeSeconds();
					Cast<ARASAIBossController>(GetController())->GetBlackboardComponent()->SetValueAsInt(BBBestSkillIndex, -1);
					UE_LOG(LogTemp, Log, TEXT("쿨타임 기록됨w"));
				});
		}
		else
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Combo")), *AttackSectionName, 1.f,
				[this, InAttackNumber](UAnimMontage* Montage, bool bInterrupted)
				{
					SkillScoreDataMap[InAttackNumber].LastUsedTime = GetWorld()->GetTimeSeconds();
					Cast<ARASAIBossController>(GetController())->GetBlackboardComponent()->SetValueAsInt(BBBestSkillIndex, -1);
					UE_LOG(LogTemp, Log, TEXT("쿨타임 기록됨e"));
				});
		}
		
		bUnflinching = true;
	}
}

void ARASBossMonster::EndAttack()
{
	Super::EndAttack();

	bUnflinching = false;
}

void ARASBossMonster::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	ARASPlayer* FromPlayer = Cast<ARASPlayer>(InFrom);
	if (FromPlayer == nullptr) return;

	Target = InFrom;

	if (float ActualDamage = Stat->ApplyDamage(InDamage) > 0)
	{
		FRotator LookAtFrom = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InFrom->GetActorLocation());
		SetActorRotation(LookAtFrom);
		Stat->ApplyStaminaDamage(InStaminaDamage);
		if (Stat->GetHp() <= 0) return;
		
		if (bUnflinching == false)
		{
			if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(MonsterAnimComponent->GetMontageByName(TEXT("Hit")))) return;
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Hit")), TEXT("Hit"), 1.0f);
		}
	}
}

void ARASBossMonster::KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower)
{
	Super::KnockbackToDirection(InFrom, Direction, InPower);
	InPower /= 5.f;
	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			MonsterAnimComponent->PlayMontageWithSection(MonsterAnimComponent->GetMontageByName(TEXT("Death")), TEXT("Knockback"), 1.0f);
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

	if (OnStopAttack.IsBound() == true)
	{
		OnStopAttack.Broadcast();
	}

	EndAttack();
}

void ARASBossMonster::Death()
{
	
}

void ARASBossMonster::ExecuteDeath(int32 InDeathNumber)
{

}

void ARASBossMonster::Dash(FVector InDireciton, float InDashSpeed)
{

}

void ARASBossMonster::Test(int InAttackNumber /*= 0*/)
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	StartAttackMontage(InAttackNumber);
}

TArray<FVector> ARASBossMonster::GetWeaponPosition()
{
	TArray<FVector> WeaponPositions;
	WeaponPositions.Add(WeaponStart->GetComponentLocation());
	WeaponPositions.Add(WeaponEnd->GetComponentLocation());

	return WeaponPositions;
}

FVector ARASBossMonster::GetCircleAttackPosition()
{
	return WeaponCircleAttack->GetComponentLocation();
}

void ARASBossMonster::SetWeaponOn(bool bWeaponOn)
{
	WeaponMesh->SetVisibility(bWeaponOn);
}

FSkillScoreData& ARASBossMonster::GetSkillScoreData(int32 InIdx)
{
	check(SkillScoreDataMap.Contains(InIdx)); 
	return SkillScoreDataMap[InIdx];
}

int32 ARASBossMonster::GetSkillScoreDataCount()
{
	return BossScoreData->SkillScoreDataMap.Num();
}

float ARASBossMonster::GetHealthPercent()
{
	return Stat->GetHp() / Stat->GetMaxHp();
}

float ARASBossMonster::GetStaminaPercent()
{
	return Stat->GetStamina() / Stat->GetMaxStamina();
}

void ARASBossMonster::InitUI()
{
	if (ShowBossHUD() == false)
	{
		ensure(false);
		return;
	}

	if (!Stat)
	{
		ensure(false);
		return;
	}

	BossHUDWidget->BindHP(Stat);
	BossHUDWidget->BindStamina(Stat);
}

bool ARASBossMonster::ShowBossHUD()
{
	if (!BossHUDWidget && BossHUDWidgetClass)
		BossHUDWidget = CreateWidget<URASBossHUDWidget>(GetWorld(), BossHUDWidgetClass);

	if (BossHUDWidget && !BossHUDWidget->IsInViewport())
	{
		BossHUDWidget->AddToViewport(10);
		return true;
	}
	return false;
}

bool ARASBossMonster::HideBossHUD()
{
	if (BossHUDWidget && BossHUDWidget->IsInViewport())
	{
		BossHUDWidget->RemoveFromParent();
		return true;
	}
	return false;
}

void ARASBossMonster::PreDeath()
{
	ARASAIBossController* MyController = Cast<ARASAIBossController>(GetController());

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCollision"));

	MyController->StopAI();
	IndicatorWideget->SetVisibility(false);

	bIsDeath = true;

	MonsterAnimComponent->StopMontage(nullptr, 0.1f);
	MonsterAnimComponent->ClearAllDelegate();
	MonsterAnimComponent->ChangeRootMotionMode(true);
}
