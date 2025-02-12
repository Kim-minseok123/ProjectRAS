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
}

void ARASPlayer::Move(const FInputActionValue& Value)
{
	FVector2D CurrentMovementInput = Value.Get<FVector2D>();

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

	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();
	if (MyAnimInstance == nullptr) return;

	if (bIsRolling) return;
	bIsRolling = true;

	if (bUseControllerRotationYaw)
	{
		LockOn();
		FVector MoveInput = GetLastMovementInputVector(); 

		if (!MoveInput.IsNearlyZero())
		{
			MoveInput.Normalize();

			FRotator DesiredRotation = MoveInput.Rotation();

			SetActorRotation(DesiredRotation);
		}

		MyAnimInstance->Montage_Play(RollMontage);
		MyAnimInstance->Montage_JumpToSection(TEXT("Roll"));
		

		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				bIsRolling = false;
				LockOn();
			});
		MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RollMontage);

	}
	else
	{
		MyAnimInstance->Montage_Play(RollMontage);
		MyAnimInstance->Montage_JumpToSection(TEXT("Roll"));
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
			{
				bIsRolling = false;
			});
		MyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, RollMontage);
	}
}

void ARASPlayer::LockOn()
{
	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = !bUseControllerRotationYaw;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

