// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/RASPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Animation//Player/RASPlayerAnimInstance.h"
#include "Component/Player/RASComboComponent.h"
#include "Component/Stat/RASStatComponent.h"
#include "Utils/RASCollisionChannels.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/RASUtils.h"
#include "Data/RASPlayerState.h"
#include "UI/RASAimWidget.h"
#include "UI/RASPlayerHUDWidget.h"
#include "Data/RASGameSingleton.h"

ARASPlayer::ARASPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/1_ProjectRAS/Animation/Player/BP_RASPlayerAnimation.BP_RASPlayerAnimation_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	HeadPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HeadPoint"));
	HeadPoint->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(HeadPoint);

	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	// Input Action
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/1_ProjectRAS/Input/IMC_NonBattle.IMC_NonBattle'"));
		if (MappingContextRef.Object)
		{
			MappingContext = MappingContextRef.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_Move.IA_Move'"));
		if (MoveActionRef.Object)
		{
			MoveAction = MoveActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_Look.IA_Look'"));
		if (LookActionRef.Object)
		{
			LookAction = LookActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> RollActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_Roll.IA_Roll'"));
		if (RollActionRef.Object)
		{
			RollAction = RollActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> LockOnActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_LockOn.IA_LockOn'"));
		if (LockOnActionRef.Object)
		{
			LockOnAction = LockOnActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> LeftAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_LeftAttack.IA_LeftAttack'"));
		if (LeftAttackActionRef.Object)
		{
			LeftAttackAction = LeftAttackActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> ShiftAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_Shift.IA_Shift'"));
		if (ShiftAttackActionRef.Object)
		{
			ShiftAttackAction = ShiftAttackActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> FAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_F.IA_F'"));
		if (FAttackActionRef.Object)
		{
			FAttackAction = FAttackActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> QAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_Q.IA_Q'"));
		if (QAttackActionRef.Object)
		{
			QAttackAction = QAttackActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> EAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_E.IA_E'"));
		if (EAttackActionRef.Object)
		{
			EAttackAction = EAttackActionRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> ParyyingActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/1_ProjectRAS/Input/Action/IA_RightClick.IA_RightClick'"));
		if (ParyyingActionRef.Object)
		{
			ParryingAction = ParyyingActionRef.Object;
		}
	}

	CombatState = EPlayerCombatState::Idle;

	bLockOn = false;

	ComboAttack = CreateDefaultSubobject<URASComboComponent>(TEXT("Combo"));

	CreatureName = TEXT("Player");

	static ConstructorHelpers::FClassFinder<URASPlayerHUDWidget> PlayerHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (PlayerHUDWidgetRef.Class)
	{
		PlayerHUDWidgetClass = PlayerHUDWidgetRef.Class;
	}
}

void ARASPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<URASPlayerHUDWidget>(GetWorld()->GetFirstPlayerController(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			Stat->BaseStats = URASGameSingleton::Get().GetStatForName(CreatureName);
			CreatureDamageInfo = URASGameSingleton::Get().GetDamageInfoForName(CreatureName);

			PlayerHUDWidget->AddToViewport();

			PlayerHUDWidget->BindHP(Stat);
			PlayerHUDWidget->BindStamina(Stat);

			Stat->SetHp(10000);
			Stat->SetStamina(10000);

			Stat->OnHpZero.AddUObject(this, &ARASPlayer::Death);
		}
	}
}

void ARASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LockOnTarget != nullptr)
	{
		float CurDistanceToTarget = GetDistanceTo(LockOnTarget);
		if (CurDistanceToTarget > 2000.f)
		{
			LockOnTarget = nullptr;
			PressTab();
			return;
		}
		FRotator CurrentControlRot = GetControlRotation();
		FVector PlayerLocation = GetActorLocation();
		FVector TargetLocation = LockOnTarget->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

		FRotator NewRot = FMath::RInterpTo(CurrentControlRot, TargetRotation, DeltaTime, 5.f);

		FRotator FinalRotation(CurrentControlRot.Pitch, NewRot.Yaw, CurrentControlRot.Roll);
		GetController()->SetControlRotation(FinalRotation);
	}
}

void ARASPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ARASPlayer::Roll);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARASPlayer::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARASPlayer::Look);
	EnhancedInput->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressTab);
	EnhancedInput->BindAction(LeftAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressComboAction);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Started, this, &ARASPlayer::PressShift);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Completed, this, &ARASPlayer::PressShiftEnd);
	EnhancedInput->BindAction(FAttackAction, ETriggerEvent::Started, this, &ARASPlayer::PressF);
	EnhancedInput->BindAction(FAttackAction, ETriggerEvent::Completed, this, &ARASPlayer::PressFEnd);
	EnhancedInput->BindAction(QAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressQ);
	EnhancedInput->BindAction(EAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressE);
	EnhancedInput->BindAction(ParryingAction, ETriggerEvent::Started, this, &ARASPlayer::PressRightClick);
	EnhancedInput->BindAction(ParryingAction, ETriggerEvent::Completed, this, &ARASPlayer::PressRightClickEnd);
}

void ARASPlayer::Move(const FInputActionValue& Value)
{
	FVector2D CurrentMovementInput = Value.Get<FVector2D>();
	LastMoveInput = CurrentMovementInput;

	if (CombatState != EPlayerCombatState::Idle) return;

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, CurrentMovementInput.X);
	AddMovementInput(RightDirection, CurrentMovementInput.Y);
}

void ARASPlayer::Look(const FInputActionValue& Value)
{
	if (CombatState == EPlayerCombatState::Deathing) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if(LockOnTarget == nullptr)
		AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARASPlayer::Roll(const FInputActionValue& Value)
{
	if (RollMontage == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr) return;
	if(CombatState == EPlayerCombatState::Rolling || CombatState == EPlayerCombatState::Breaking) return;
	if (Stat->GetStamina() <= 0) return;
	if (CombatState == EPlayerCombatState::Deathing) return;

	CombatState = EPlayerCombatState::Rolling;

	AnimInstance->Montage_Stop(0.1f);

	FName RollSection = TEXT("Roll_F"); // 기본 섹션

	// 입력값이 없으면 기본값 (정면, (1,0)) 사용
	FVector2D RawInput;
	if (!LastMoveInput.IsNearlyZero())
	{
		RawInput = LastMoveInput.GetSafeNormal();
	}
	else
	{
		RawInput = FVector2D(1.f, 0.f);
	}

	float SignedAngle = 0.f; // 계산될 각도 (0~360)

	if (bUseControllerRotationYaw)
	{
		// 카메라 회전 기준: 원시 입력을 카메라 기준으로 월드 방향으로 변환
		FRotator CamRot = Controller->GetControlRotation();
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
		FRotator CamRot = Controller->GetControlRotation();
		FRotator CamYaw(0, CamRot.Yaw, 0);
		FVector IntendedDirection = FRotationMatrix(CamYaw).TransformVector(FVector(RawInput.X, RawInput.Y, 0.f));
		IntendedDirection.Normalize();

		// 캐릭터의 현재 정면 (Actor의 forward)
		FVector ActorForward = GetActorForwardVector();
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

	AnimInstance->Montage_Play(RollMontage);
	AnimInstance->Montage_JumpToSection(RollSection);

	Stat->ApplyStaminaDamage(30.f);

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (ComboAttack)
			{
				ComboAttack->EndCombo(true, .5f);
			}
		});
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RollMontage);
}

void ARASPlayer::PressTab()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

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

void ARASPlayer::LockOn()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = true;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::LockOff()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = false;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::SetInBattleTimer()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

	GetWorld()->GetTimerManager().ClearTimer(BattleTimer);
	SetInBattle(true);
	GetWorld()->GetTimerManager().SetTimer(BattleTimer, [this]()
		{
			SetInBattle(false);
		}, 4.f, false);
}

void ARASPlayer::PressComboAction()
{
	if (CombatState == EPlayerCombatState::Deathing) return;

	if (CombatState == EPlayerCombatState::Idle || CombatState == EPlayerCombatState::Attacking)
	{
		if (ComboAttack)
		{
			if (Stat->GetStamina() <= 0) return;

			SetInBattleTimer();
			if (bIsPressShift)
				ComboAttack->PressComboAction(EAttackType::Shift);
			else if (bIsPressF)
				ComboAttack->PressComboAction(EAttackType::F);
			else
				ComboAttack->PressComboAction(EAttackType::LeftClick);
		}
	}
}

void ARASPlayer::PressShift()
{
	bIsPressShift = true;
}

void ARASPlayer::PressShiftEnd()
{
	bIsPressShift = false;
}

void ARASPlayer::PressF()
{
	bIsPressF = true;
}

void ARASPlayer::PressFEnd()
{
	bIsPressF = false;
}

void ARASPlayer::PressQ()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	CombatState = EPlayerCombatState::Skilling; 
	SetInBattleTimer();

	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (MyAnimInstance == nullptr)
		return;

	MyAnimInstance->Montage_Play(SkillMontage);
	MyAnimInstance->Montage_JumpToSection(TEXT("Skill1"), SkillMontage);

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this, MyAnimInstance](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (ComboAttack)
			{
				ComboAttack->EndCombo(true, 0.9f);
			}
		});
	MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillMontage);
}

void ARASPlayer::PressE()
{
	if (CombatState != EPlayerCombatState::Idle)
		return;

	CombatState = EPlayerCombatState::Skilling;
	SetInBattleTimer();

	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (MyAnimInstance == nullptr)
		return;

	MyAnimInstance->Montage_Play(SkillMontage);
	MyAnimInstance->Montage_JumpToSection(TEXT("Skill2"), SkillMontage);

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this, MyAnimInstance](UAnimMontage* Montage, bool bInterrupted)
		{
			CombatState = EPlayerCombatState::Idle;
			if (ComboAttack)
			{
				ComboAttack->EndCombo(true, 0.9f);
			}
		});
	MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillMontage);
}

void ARASPlayer::PressRightClick()
{
	if (CombatState == EPlayerCombatState::Breaking || CombatState == EPlayerCombatState::Armoring)
		return;
	if (CombatState != EPlayerCombatState::Idle)
		return;
	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (!MyAnimInstance)
		return;
	SetInBattleTimer();

	ComboAttack->EndCombo(false, 0.f);

	CombatState = EPlayerCombatState::Parrying;

	MyAnimInstance->Montage_Play(ParryingMontage);
	ParryingTime = RASUtils::GetCurrentPlatformTime();
	MyAnimInstance->Montage_JumpToSection(TEXT("Parrying"), ParryingMontage);

}

void ARASPlayer::PressRightClickEnd()
{
	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (!MyAnimInstance)
		return;

	if (CombatState == EPlayerCombatState::Breaking || CombatState == EPlayerCombatState::Armoring || CombatState == EPlayerCombatState::Deathing)
		return;

	UAnimMontage* CurrentMontage = MyAnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage)
	{
		FName CurrentSection = MyAnimInstance->Montage_GetCurrentSection(CurrentMontage);
		if (CurrentSection != TEXT("ParryingBreak") || CurrentSection != TEXT("ParryingExact"))
		{
			CombatState = EPlayerCombatState::Idle;

			MyAnimInstance->Montage_Stop(0.25f, ParryingMontage);
		}
	}
}

void ARASPlayer::FindAllEnemyInRange()
{
	TargetEnemys.Empty();

	FVector Center = GetActorLocation();
	const float SphereRadius = 2000.f;

	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(SphereRadius);
	FCollisionQueryParams Params(FName(TEXT("FindEnemy")), false, this);

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

void ARASPlayer::SetClosestLockedOnTarget()
{
	if (LockOnTarget != nullptr)
		return;

	ARASCharacterBase* Target = nullptr;
	float MinDistance = 21000.f;
	for (ARASCharacterBase* Enemy : TargetEnemys)
	{
		float CurDistance = GetDistanceTo(Enemy);
		if (CurDistance < MinDistance)
		{
			MinDistance = CurDistance;
			Target = Enemy;
		}
	}

	SetLockedOnTarget(Target);
}

void ARASPlayer::SetLockedOnTarget(ARASCharacterBase* Target)
{
	if (LockOnTarget)
	{
		LockOnTarget->SetVisibleIndicator(false);
	}
	LockOnTarget = Target;

	if (LockOnTarget)
	{
		LockOnTarget->SetVisibleIndicator(true);
		LockOn();
	}
	else
	{
		LockOff();
	}
}

void ARASPlayer::CycleLockOnTarget()
{
	if (TargetEnemys.Num() == 0)
		return;

	TArray<ARASCharacterBase*> EnemyArray = TargetEnemys.Array();

	EnemyArray.Sort([this](const ARASCharacterBase& A, const ARASCharacterBase& B)
		{
			return GetDistanceTo(&A) < GetDistanceTo(&B);
		});

	int32 CurrentIndex = EnemyArray.IndexOfByKey(LockOnTarget);
	int32 NextIndex = (CurrentIndex + 1) % EnemyArray.Num();

	SetLockedOnTarget(EnemyArray[NextIndex]);
}

void ARASPlayer::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	if (CombatState == EPlayerCombatState::Rolling || CombatState == EPlayerCombatState::Armoring || CombatState == EPlayerCombatState::Deathing)
		return;
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);
	if (LockOnTarget == nullptr)
		SetLockedOnTarget(InFrom);

	SetInBattleTimer();
	if (CombatState == EPlayerCombatState::Parrying)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance)
			return;
		Stat->ApplyStaminaDamage(InStaminaDamage);
		float CurrentTime = RASUtils::GetCurrentPlatformTime();
		UE_LOG(LogTemp, Log, TEXT("Parrying time diff: %f"), CurrentTime - ParryingTime);

		if (CurrentTime - ParryingTime <= 0.1f)
		{
			AnimInstance->Montage_Play(ParryingMontage);
			AnimInstance->Montage_JumpToSection(TEXT("ParryingExact"), ParryingMontage);
			CombatState = EPlayerCombatState::Armoring;

			GetWorldSettings()->SetTimeDilation(0.6f);
			FTimerHandle TimeDilationTimer;
			GetWorld()->GetTimerManager().SetTimer(TimeDilationTimer, [this]()
				{
					GetWorldSettings()->SetTimeDilation(1.f);
				}, 0.8f, false);

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindLambda([this, AnimInstance](UAnimMontage* Montage, bool bInterrupted)
				{
					CombatState = EPlayerCombatState::Idle;
				});
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ParryingMontage);
			return;
		}
		if (Stat->GetStamina() <= 0)
		{
			AnimInstance->Montage_Play(ParryingMontage);
			AnimInstance->Montage_JumpToSection(TEXT("ParryingBreak"), ParryingMontage);
			CombatState = EPlayerCombatState::Breaking;
			GetWorldSettings()->SetTimeDilation(0.7f);
			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindLambda([this, AnimInstance](UAnimMontage* Montage, bool bInterrupted)
				{
					CombatState = EPlayerCombatState::Idle;
					GetWorldSettings()->SetTimeDilation(1.f);
				});
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ParryingMontage);
		}
		else
		{
			AnimInstance->Montage_Play(ParryingMontage);
			AnimInstance->Montage_JumpToSection(TEXT("ParryingHit"), ParryingMontage);
		}
	}
	else
	{
		float ActualDamage = Stat->ApplyDamage(InDamage);
		if (ActualDamage > 0)
		{
			if (Stat->GetHp() > 0)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (!AnimInstance)
					return;
				AnimInstance->Montage_Stop(0.1f);
				ComboAttack->EndCombo(true, 1.f);
				AnimInstance->Montage_Play(HitMontage);
				CombatState = EPlayerCombatState::Breaking;
				if (ActualDamage >= KnockbackFigure)
				{
					AnimInstance->Montage_JumpToSection(TEXT("Knockback"), HitMontage);
				}
				else
				{
					AnimInstance->Montage_JumpToSection(TEXT("Hit"), HitMontage);
				}
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindLambda([this, AnimInstance](UAnimMontage* Montage, bool bInterrupted)
					{
						CombatState = EPlayerCombatState::Idle;
					});
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, HitMontage);
			}
		}
	}
}

void ARASPlayer::Death()
{
	CombatState = EPlayerCombatState::Deathing;

	Super::Death();

	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			// 플레이어 사망 UI 띄움, UI에서 다시하기 버튼 클릭시  게임모드에서 플레이어를 저장된 위치로 리스폰
		}, 
		4.f, false);
}
