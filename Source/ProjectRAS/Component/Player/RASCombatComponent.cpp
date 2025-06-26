// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASCombatComponent.h"
#include "Component/Stat/RASStatComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASPlayerAnimComponent.h"
#include "Component/Player/RASComboComponent.h"
#include "Utils/RASCollisionChannels.h"
#include "Engine/OverlapResult.h"
#include "Utils/RASUtils.h"
#include "Component/Player/RASUIComponent.h"
#include "Controller/Player/RASPlayerController.h"

// Sets default values for this component's properties
URASCombatComponent::URASCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void URASCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = GetOwner<ARASPlayer>();
}

void URASCombatComponent::FindAllEnemyInRange()
{
	TargetEnemys.Empty();

	FVector Center = OwnerPlayer->GetActorLocation();
	const float SphereRadius = 2000.f;

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(SphereRadius);
	FCollisionQueryParams Params(FName(TEXT("FindEnemy")), false, OwnerPlayer);

	TArray<FOverlapResult> Results;

	DrawDebugSphere(GetWorld(), Center, SphereRadius, 12, FColor::Green, false, 1.0f, 0, 2.0f);

	bool bIsHit = GetWorld()->OverlapMultiByChannel(
		Results,
		Center,
		FQuat::Identity,
		ECC_RASChannel,
		CollisionSphere,
		Params
	);

	if (bIsHit)
	{
		for (const FOverlapResult& Result : Results)
		{
			ARASCharacterBase* FindEnemy = Cast<ARASCharacterBase>(Result.GetActor());
			if (FindEnemy != nullptr)
			{
				TargetEnemys.Add(FindEnemy);
			}
		}
	}
}

void URASCombatComponent::SetClosestLockedOnTarget()
{
	if (LockOnTarget != nullptr)
		return;

	ARASCharacterBase* Target = nullptr;
	float MinDistance = 21000.f;
	for (ARASCharacterBase* Enemy : TargetEnemys)
	{
		float CurDistance = OwnerPlayer->GetDistanceTo(Enemy);
		if (CurDistance < MinDistance)
		{
			MinDistance = CurDistance;
			Target = Enemy;
		}
	}

	SetLockedOnTarget(Target);
}

void URASCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URASCombatComponent::SetInBattleTimer()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

	OwnerPlayer->GetWorld()->GetTimerManager().ClearTimer(BattleTimer);
	SetInBattle(true);
	OwnerPlayer->GetWorld()->GetTimerManager().SetTimer(BattleTimer, [this]()
		{
			SetInBattle(false);
		}, 4.f, false);
}

void URASCombatComponent::Roll(const FInputActionValue& Value)
{
	if (OwnerPlayer == nullptr) return;

	URASPlayerAnimComponent* AnimComponent = OwnerPlayer->GetAnimComponent();
	if(AnimComponent == nullptr) return;

	if (CombatState == EPlayerCombatState::Rolling || CombatState == EPlayerCombatState::Breaking || CombatState == EPlayerCombatState::Deathing || CombatState == EPlayerCombatState::UsingItem || CombatState == EPlayerCombatState::Executing) return;
	if (OwnerPlayer->GetStat()->GetStamina() < 30) return;

	CombatState = EPlayerCombatState::Rolling;

	AnimComponent->StopMontage(nullptr, 0.1f);

	FName RollSection = TEXT("Roll_F"); // 기본 섹션

	// 입력값이 없으면 기본값 (정면, (1,0)) 사용
	FVector2D RawInput;
	FVector2D LastMoveInput = OwnerPlayer->GetLastMoveInput();

	if (!LastMoveInput.IsNearlyZero())
	{
		RawInput = LastMoveInput.GetSafeNormal();
	}
	else
	{
		RawInput = FVector2D(1.f, 0.f);
	}

	float SignedAngle = 0.f; // 계산될 각도 (0~360)

	if (OwnerPlayer->bUseControllerRotationYaw)
	{
		// 카메라 회전 기준: 원시 입력을 카메라 기준으로 월드 방향으로 변환
		FRotator CamRot = OwnerPlayer->Controller->GetControlRotation();
		FRotator CamYaw(0, CamRot.Yaw, 0);
		FVector RollDirection = FRotationMatrix(CamYaw).TransformVector(FVector(RawInput.X, RawInput.Y, 0.f));
		RollDirection.Normalize();

		// 기준 정면은 카메라의 forward (CamYaw의 X축)
		FVector ReferenceForward = FRotationMatrix(CamYaw).GetUnitAxis(EAxis::X);

		float Dot = FVector::DotProduct(ReferenceForward, RollDirection);
		float AngleDegrees = FMath::Acos(Dot) * (180.f / PI);
		FVector Cross = FVector::CrossProduct(ReferenceForward, RollDirection);
		float Sign = (Cross.Z >= 0.f) ? 1.f : -1.f;
		SignedAngle = AngleDegrees * Sign;
		if (SignedAngle < 0.f)
		{
			SignedAngle += 360.f;
		}
	}
	else
	{
		// bUseControllerRotationYaw == false: 캐릭터가 직접 회전한 상태.
		// 원시 입력은 카메라 기준으로 저장되어 있으므로, 의도한 이동 방향을
		// 카메라의 회전(원래 입력 기준)으로부터 구함.
		FRotator CamRot = OwnerPlayer->Controller->GetControlRotation();
		FRotator CamYaw(0, CamRot.Yaw, 0);
		FVector IntendedDirection = FRotationMatrix(CamYaw).TransformVector(FVector(RawInput.X, RawInput.Y, 0.f));
		IntendedDirection.Normalize();

		// 캐릭터의 현재 정면 (Actor의 forward)
		FVector ActorForward = OwnerPlayer->GetActorForwardVector();
		ActorForward = ActorForward.GetSafeNormal2D();

		float Dot = FVector::DotProduct(ActorForward, IntendedDirection);
		float AngleDegrees = FMath::Acos(Dot) * (180.f / PI);
		FVector Cross = FVector::CrossProduct(ActorForward, IntendedDirection);
		float Sign = (Cross.Z >= 0.f) ? 1.f : -1.f;
		SignedAngle = AngleDegrees * Sign;
		if (SignedAngle < 0.f)
		{
			SignedAngle += 360.f;
		}
	}

	// 8방향(45도 단위) 매핑
	if ((SignedAngle >= 337.5f && SignedAngle < 360.f) || (SignedAngle >= 0.f && SignedAngle < 22.5f))
	{
		RollSection = TEXT("Roll_F");  // 정면
	}
	else if (SignedAngle >= 22.5f && SignedAngle < 67.5f)
	{
		RollSection = TEXT("Roll_FR"); // 정면 우측 대각
	}
	else if (SignedAngle >= 67.5f && SignedAngle < 112.5f)
	{
		RollSection = TEXT("Roll_R");  // 우측
	}
	else if (SignedAngle >= 112.5f && SignedAngle < 157.5f)
	{
		RollSection = TEXT("Roll_BR"); // 후면 우측 대각
	}
	else if (SignedAngle >= 157.5f && SignedAngle < 202.5f)
	{
		RollSection = TEXT("Roll_B");  // 후면
	}
	else if (SignedAngle >= 202.5f && SignedAngle < 247.5f)
	{
		RollSection = TEXT("Roll_BL"); // 후면 좌측 대각
	}
	else if (SignedAngle >= 247.5f && SignedAngle < 292.5f)
	{
		RollSection = TEXT("Roll_L");  // 좌측
	}
	else if (SignedAngle >= 292.5f && SignedAngle < 337.5f)
	{
		RollSection = TEXT("Roll_FL"); // 정면 좌측 대각
	}

	OwnerPlayer->GetStat()->ApplyStaminaDamage(30.f);

	AnimComponent->PlayMontageWithSection(AnimComponent->GetMontageByName(TEXT("Roll")), RollSection, 1.2f,
		[this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (OwnerPlayer && OwnerPlayer->GetComboComponent())
			{
				OwnerPlayer->GetComboComponent()->EndCombo(true, 0.1f);
			}
		});
}

void URASCombatComponent::PressTab()
{
	if (CombatState == EPlayerCombatState::Deathing || CombatState == EPlayerCombatState::Executing) return;

	FindAllEnemyInRange();
	if (TargetEnemys.IsEmpty())
		LockOnTarget = nullptr;

	if (LockOnTarget == nullptr)
	{
		SetClosestLockedOnTarget();
	}
	else
	{
		CycleLockOnTarget();
	}
}

void URASCombatComponent::PressComboAction()
{
	if (CombatState == EPlayerCombatState::Deathing) return;
	if (CombatState == EPlayerCombatState::Executing) return;

	if (CombatState == EPlayerCombatState::Idle || CombatState == EPlayerCombatState::Attacking)
	{
		if (URASComboComponent* ComboAttack = OwnerPlayer->GetComboComponent())
		{
			if (OwnerPlayer->GetStat()->GetStamina() <= 0) return;
			SetInBattleTimer();

			if (bIsPressShift)
				ComboAttack->PressComboAction(EAttackType::Shift);
			else
				ComboAttack->PressComboAction(EAttackType::LeftClick);
		}
	}
}

void URASCombatComponent::PressShift()
{
	bIsPressShift = true;
}

void URASCombatComponent::PressShiftEnd()
{
	bIsPressShift = false;
}

void URASCombatComponent::PressF()
{
	URASComboComponent* ComboAttack = OwnerPlayer->GetComboComponent();
	ComboAttack->EndCombo();
	if (CombatState == EPlayerCombatState::Deathing) return;
	if (CombatState == EPlayerCombatState::Idle || CombatState == EPlayerCombatState::Attacking)
	{
		if (OwnerPlayer->GetStat()->GetStamina() <= 0) return;
		SetInBattleTimer();

		if (LockOnTarget != nullptr && LockOnTarget->GetTotalStamina() <= 0.f && OwnerPlayer->GetDistanceTo(LockOnTarget) <= 200.f)
		{
			if (LockOnTarget->GetCreatureName().IsEqual(TEXT("Ashur"))) return; 
			if (CombatState != EPlayerCombatState::Executing)
			{
				CombatState = EPlayerCombatState::Executing;
				KillTarget(LockOnTarget);
			}
		}
	}
}

void URASCombatComponent::PressQ()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	if(GetWorld()->GetTimerManager().IsTimerActive(SkillQTimer))
		return;

	CombatState = EPlayerCombatState::Skilling;
	SetInBattleTimer();

	GetWorld()->GetTimerManager().SetTimer(
		SkillQTimer,
		[this]()
		{
			
		},
		9.f,
		false);

	OwnerPlayer->GetUIComponent()->SetIconProgressBar(1, 9.f);
	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Skill")), TEXT("Skill1"), 1.2f,
		[this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (OwnerPlayer && OwnerPlayer->GetComboComponent())
			{
				OwnerPlayer->GetComboComponent()->EndCombo(true, 0.7f);
			}
		});
}

void URASCombatComponent::PressE()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	if (GetWorld()->GetTimerManager().IsTimerActive(SkillETimer))
		return;

	CombatState = EPlayerCombatState::Skilling;
	SetInBattleTimer();

	GetWorld()->GetTimerManager().SetTimer(
		SkillETimer,
		[this]()
		{

		},
		10.f,
		false);
	OwnerPlayer->GetUIComponent()->SetIconProgressBar(2, 10.f);

	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Skill")), TEXT("Skill2"), 1.2f,
		[this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (OwnerPlayer && OwnerPlayer->GetComboComponent())
			{
				OwnerPlayer->GetComboComponent()->EndCombo(true, 0.7f);
			}
		});
}

void URASCombatComponent::PressRightClick()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	SetInBattleTimer();

	OwnerPlayer->GetComboComponent()->EndCombo(false, 0.f);
	if (CombatState != EPlayerCombatState::Idle)
		return;
	CombatState = EPlayerCombatState::Parrying;

	ParryingTime = RASUtils::GetCurrentPlatformTime();
	MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Parrying")), TEXT("Parrying"), 1.0f);
}

void URASCombatComponent::PressRightClickHold()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	PressRightClick();
}

void URASCombatComponent::PressRightClickEnd()
{
	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	if (CombatState == EPlayerCombatState::Breaking || CombatState == EPlayerCombatState::Armoring || CombatState == EPlayerCombatState::Deathing || CombatState == EPlayerCombatState::Executing)
		return;

	if(MyAnimInstance->StopParryingAnimation())
		CombatState = EPlayerCombatState::Idle;
}

void URASCombatComponent::SetLockedOnTarget(ARASCharacterBase* Target)
{
	if (LockOnTarget)
	{
		LockOnTarget->SetVisibleIndicator(false);
	}
	LockOnTarget = Target;

	if (LockOnTarget)
	{
		LockOnTarget->SetVisibleIndicator(true);
		OwnerPlayer->LockOn();
	}
	else
	{
		OwnerPlayer->LockOff();
	}
}

void URASCombatComponent::CycleLockOnTarget()
{
	if (TargetEnemys.Num() == 0)
		return;

	TArray<ARASCharacterBase*> EnemyArray = TargetEnemys.Array();

	EnemyArray.Sort([this](const ARASCharacterBase& A, const ARASCharacterBase& B)
		{
			return OwnerPlayer->GetDistanceTo(&A) < OwnerPlayer->GetDistanceTo(&B);
		});

	int32 CurrentIndex = EnemyArray.IndexOfByKey(LockOnTarget);
	int32 NextIndex = (CurrentIndex + 1) % EnemyArray.Num();

	SetLockedOnTarget(EnemyArray[NextIndex]);
}

void URASCombatComponent::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	if (CombatState == EPlayerCombatState::Rolling || CombatState == EPlayerCombatState::Armoring || CombatState == EPlayerCombatState::Deathing || CombatState == EPlayerCombatState::Executing)
		return;

	if (LockOnTarget == nullptr)
		SetLockedOnTarget(InFrom);

	SetInBattleTimer();

	URASStatComponent* Stat = OwnerPlayer->GetStat();
	if (Stat == nullptr) return;

	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	if (InDamage == 999.f)
	{
		float ActualDamage = Stat->ApplyDamage(InDamage);
		if (ActualDamage > 0)
		{
			if (Stat->GetHp() > 0)
			{
				//if (CombatState == EPlayerCombatState::Breaking) return;
				MyAnimInstance->StopMontage(nullptr, 0.1f);

				URASComboComponent* ComboAttack = OwnerPlayer->GetComboComponent();
				ComboAttack->EndCombo(true, 0.8f);

				CombatState = EPlayerCombatState::Breaking;

				MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Hit")), TEXT("Hit"), 1.0f,
					[this](UAnimMontage* Montage, bool bInterrupted)
					{
						CombatState = EPlayerCombatState::Idle;
					});
			}
		}
		return;
	}
	
	if (CombatState == EPlayerCombatState::Parrying)
	{
		Stat->ApplyStaminaDamage(InStaminaDamage);
		float CurrentTime = RASUtils::GetCurrentPlatformTime();
		UE_LOG(LogTemp, Log, TEXT("Parrying time diff: %f"), CurrentTime - ParryingTime);

		if (CurrentTime - ParryingTime <= 0.1f)
		{
			CombatState = EPlayerCombatState::Armoring;

			OwnerPlayer->GetWorldSettings()->SetTimeDilation(0.6f);
			Stat->ApplyStaminaDamage(30.f);
			FTimerHandle TimeDilationTimer;
			OwnerPlayer->GetWorld()->GetTimerManager().SetTimer(TimeDilationTimer, [this]()
				{
					OwnerPlayer->GetWorldSettings()->SetTimeDilation(1.f);
				}, 0.8f, false);

			MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Parrying")), TEXT("ParryingExact"), 1.0f,
				[this](UAnimMontage* Montage, bool bInterrupted)
				{
					CombatState = EPlayerCombatState::Idle;
				});

			return;
		}
		if (Stat->GetStamina() <= 0)
		{
			CombatState = EPlayerCombatState::Breaking;
			OwnerPlayer->GetWorldSettings()->SetTimeDilation(0.7f);

			MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Parrying")), TEXT("ParryingBreak"), 1.0f,
				[this](UAnimMontage* Montage, bool bInterrupted)
				{
					CombatState = EPlayerCombatState::Idle;
					OwnerPlayer->GetWorldSettings()->SetTimeDilation(1.f);
				});
		}
		else
		{
			MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Parrying")), TEXT("ParryingHit"), 1.0f);
		}
	}
	else
	{
		float ActualDamage = Stat->ApplyDamage(InDamage);
		if (ActualDamage > 0)
		{
			if (Stat->GetHp() > 0)
			{
				//if (CombatState == EPlayerCombatState::Breaking) return;
				MyAnimInstance->StopMontage(nullptr, 0.1f);

				URASComboComponent* ComboAttack = OwnerPlayer->GetComboComponent();
				ComboAttack->EndCombo(true, 0.8f);

				CombatState = EPlayerCombatState::Breaking;

				MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Hit")), TEXT("Hit"), 1.0f,
					[this](UAnimMontage* Montage, bool bInterrupted)
					{
						CombatState = EPlayerCombatState::Idle;
					});
			}
		}
	}
}

void URASCombatComponent::KillTarget(ARASCharacterBase* Target)
{
	if (Target == nullptr) return;
	
	URASStatComponent* Stat = OwnerPlayer->GetStat();
	if (Stat == nullptr) return;

	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;

	LockOnTarget = nullptr;
	int32 DeathNumber = FMath::RandRange(1, 2);
	FString MontageSectionName = FString::Printf(TEXT("Execute%d"), DeathNumber);

	MyAnimInstance->StopMontage(nullptr, 0.1f);
	MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Execute")), FName(*MontageSectionName), 1.f,
		[this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (OwnerPlayer->GetComboComponent())
			{
				PressTab();
				OwnerPlayer->GetComboComponent()->EndCombo(true, .8f);
			}
		});

	Target->ExecuteDeath(DeathNumber);
}

void URASCombatComponent::Death()
{
	if (CombatState == EPlayerCombatState::Deathing) return;
	SetCombatState(EPlayerCombatState::Deathing);

	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(OwnerPlayer->GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}

	URASPlayerAnimComponent* PlayerAnimComponent = OwnerPlayer->GetAnimComponent();

	PlayerAnimComponent->StopMontage(nullptr, 0.1f);
	PlayerAnimComponent->ClearAllDelegate();
	PlayerAnimComponent->ChangeRootMotionMode(true);

	int DeathType = FMath::RandRange(0, 1);

	if (DeathType == 0)
	{
		PlayerAnimComponent->PlayMontageWithSection(PlayerAnimComponent->GetMontageByName(TEXT("Death")), TEXT("FrontDeath"), 1.f);
	}
	else
	{
		PlayerAnimComponent->PlayMontageWithSection(PlayerAnimComponent->GetMontageByName(TEXT("Death")), TEXT("BackDeath"), 1.f);
	}

	LockOnTarget = nullptr;

	FTimerHandle DeathHandle;
	OwnerPlayer->GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			// 플레이어 사망 UI 띄움, UI에서 다시하기 버튼 클릭시  게임모드에서 플레이어를 저장된 위치로 리스폰
		},
		4.f, false);
}

void URASCombatComponent::UsePotion()
{
	if (CombatState != EPlayerCombatState::Idle) return;
	if (PotionCount <= 0) return;
	if (GetWorld()->GetTimerManager().IsTimerActive(PotionTimer)) return;
	URASStatComponent* Stat = OwnerPlayer->GetStat();
	if (Stat == nullptr) return;

	GetWorld()->GetTimerManager().SetTimer(
		PotionTimer,
		[this]()
		{

		},
		3.f,
		false);
	PotionCount--;
	OwnerPlayer->GetUIComponent()->SetIconProgressBar(3, 3.f, PotionCount);
	
	Stat->SetHp(Stat->GetHp() + Stat->GetMaxHp() * 0.6f);
	CombatState = EPlayerCombatState::UsingItem;

	URASPlayerAnimComponent* MyAnimInstance = OwnerPlayer->GetAnimComponent();
	if (MyAnimInstance == nullptr) return;
	MyAnimInstance->PlayMontageWithSection(MyAnimInstance->GetMontageByName(TEXT("Potion")), TEXT("Potion"), 1.f,
		[this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
		});
}

