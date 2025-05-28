// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/RASPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
#include "Controller/Player/RASPlayerController.h"
#include "Camera/CameraActor.h"
#include "Component/Player/RASInputComponent.h"
#include "Component/Player/RASCombatComponent.h"
#include "Component/Player/RASPlayerAnimComponent.h"
#include "Component/Player/RASUIComponent.h"
#include "Component/Player/RASCameraComponent.h"

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
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	// Input Component
	PlayerInput = CreateDefaultSubobject<URASInputComponent>(TEXT("InputComponent"));
	
	// Combat Component
	PlayerCombatComponent = CreateDefaultSubobject<URASCombatComponent>(TEXT("CombatComponent"));

	// Animation Component
	PlayerAnimComponent = CreateDefaultSubobject<URASPlayerAnimComponent>(TEXT("PlayerAnimComponent"));

	// ComboAttack Component
	PlayerComboAttack = CreateDefaultSubobject<URASComboComponent>(TEXT("Combo"));

	// UI Component
	PlayerUIComponent = CreateDefaultSubobject<URASUIComponent>(TEXT("UIComponent"));

	// Camera Component
	PlayerCameraComponent = CreateDefaultSubobject<URASCameraComponent>(TEXT("CameraComponent"));

	CreatureName = TEXT("Player");
}

void ARASPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnimComponent->SetAnimInstance(GetMesh()->GetAnimInstance());
	PlayerCombatComponent->SetCombatState(EPlayerCombatState::Idle);

	Stat->BaseStats = URASGameSingleton::Get().GetStatForName(CreatureName);
	CreatureDamageInfo = URASGameSingleton::Get().GetDamageInfoForName(CreatureName);
	PlayerUIComponent->InitUI();

	Stat->SetHp(10000);
	Stat->SetStamina(10000);

	Stat->OnHpZero.AddUObject(this, &ARASPlayer::Death);

	PlayerCameraComponent->InitCamera();
}

void ARASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ARASCharacterBase* LockOnTarget = PlayerCombatComponent->GetLockedOnTarget();
	if (LockOnTarget != nullptr && PlayerCombatComponent->GetCombatState() != EPlayerCombatState::Executing)
	{
		PlayerCameraComponent->LockOnCamera(LockOnTarget, DeltaTime);
	}
}

void ARASPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{
		PlayerInput->InitPlayerInputComponent(PlayerInputComponent);
	}
}

void ARASPlayer::Move(const FInputActionValue& Value)
{
	FVector2D CurrentMovementInput = Value.Get<FVector2D>();
	LastMoveInput = CurrentMovementInput;
	if(PlayerCombatComponent->GetCombatState() != EPlayerCombatState::UsingItem)
		if (PlayerCombatComponent->GetCombatState() != EPlayerCombatState::Idle) return;

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, CurrentMovementInput.X);
	AddMovementInput(RightDirection, CurrentMovementInput.Y);
}

void ARASPlayer::Look(const FInputActionValue& Value)
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing || PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if(PlayerCombatComponent->GetLockedOnTarget() == nullptr)
		AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARASPlayer::LockOn()
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing || PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;


	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = true;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::LockOff()
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing || PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;


	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = false;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::SwitchToExecutionCamera(float BlendTime)
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing) return;

	PlayerCameraComponent->SwitchToExecutionCamera(BlendTime);
}

void ARASPlayer::SwitchBackToOriginalCamera(float BlendTime)
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing) return;

	PlayerCameraComponent->SwitchBackToOriginalCamera(BlendTime);
}

void ARASPlayer::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	if (PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Rolling || PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Armoring || PlayerCombatComponent->GetCombatState() == EPlayerCombatState::Deathing)
		return;
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	PlayerCombatComponent->HitFromActor(InFrom, InDamage, InStaminaDamage);
}

void ARASPlayer::Death()
{
	Super::Death();

	PlayerCombatComponent->Death();
}
