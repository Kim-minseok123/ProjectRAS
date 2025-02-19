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
#include "Component/Player/ComboAttackComponent.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

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
	}

	ComboAttack = CreateDefaultSubobject<UComboAttackComponent>(TEXT("Combo"));
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
}

void ARASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARASPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ARASPlayer::Roll);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARASPlayer::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARASPlayer::Look);
	EnhancedInput->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &ARASPlayer::LockOn);
	EnhancedInput->BindAction(LeftAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressComboAction);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Started, this, &ARASPlayer::PressShift);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Completed, this, &ARASPlayer::PressShiftEnd);
	EnhancedInput->BindAction(FAttackAction, ETriggerEvent::Started, this, &ARASPlayer::PressF);
	EnhancedInput->BindAction(FAttackAction, ETriggerEvent::Completed, this, &ARASPlayer::PressFEnd);
	EnhancedInput->BindAction(QAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressQ);
	EnhancedInput->BindAction(EAttackAction, ETriggerEvent::Triggered, this, &ARASPlayer::PressE);
}

void ARASPlayer::Move(const FInputActionValue& Value)
{
	FVector2D CurrentMovementInput = Value.Get<FVector2D>();
	LastMoveInput = CurrentMovementInput;

	if (bIsRolling || bIsAttacking) return;

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, CurrentMovementInput.X);
	AddMovementInput(RightDirection, CurrentMovementInput.Y);
}

void ARASPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARASPlayer::Roll(const FInputActionValue& Value)
{
	if (RollMontage == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr) return;

	if (bIsRolling) return;
	bIsRolling = true;

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

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			bIsRolling = false;
			bIsAttacking = false;
			if (ComboAttack)
			{
				ComboAttack->EndCombo();
			}
		});
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RollMontage);
}


void ARASPlayer::LockOn()
{
	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = !bUseControllerRotationYaw;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::PressComboAction()
{
	if (ComboAttack)
	{
		if(bIsPressShift)
			ComboAttack->PressComboAction(EAttackType::Shift);
		else if(bIsPressF)
			ComboAttack->PressComboAction(EAttackType::F);
		else
			ComboAttack->PressComboAction(EAttackType::LeftClick);
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
	if (bIsRolling || bIsAttacking) return;
	bIsAttacking = true;

	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (MyAnimInstance == nullptr) return;

	MyAnimInstance->Montage_Play(SkillMontage);
	MyAnimInstance->Montage_JumpToSection(TEXT("Skill1"));
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			bIsAttacking = false;
			ComboAttack->EndCombo();
		});
	MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillMontage);
}

void ARASPlayer::PressE()
{
	if (bIsRolling || bIsAttacking) return;
	bIsAttacking = true;

	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (MyAnimInstance == nullptr) return;

	MyAnimInstance->Montage_Play(SkillMontage);
	MyAnimInstance->Montage_JumpToSection(TEXT("Skill2"));
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			bIsAttacking = false;
			ComboAttack->EndCombo();
		});
	MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillMontage);
}

