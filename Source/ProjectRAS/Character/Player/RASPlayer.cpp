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

	// Input Component
	PlayerInput = CreateDefaultSubobject<URASInputComponent>(TEXT("InputComponent"));
	
	// Combat Component
	CombatComponent = CreateDefaultSubobject<URASCombatComponent>(TEXT("CombatComponent"));

	// Animation Component
	PlayerAnimComponent = CreateDefaultSubobject<URASPlayerAnimComponent>(TEXT("PlayerAnimComponent"));

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

	PlayerAnimComponent->SetAnimInstance(GetMesh()->GetAnimInstance());
	CombatComponent->SetCombatState(EPlayerCombatState::Idle);

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
	if (GetWorld())
	{
		FVector PlayerLocation = GetActorLocation();
		FVector SpawnOffset(0.f, 218.7f, 56.3f);
		FVector SpawnLocation = PlayerLocation + SpawnOffset;

		FRotator SpawnRotation(-5.f, -50.f, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ExecuteCameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

		if (ExecuteCameraActor)
		{
			ExecuteCameraActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			UCameraComponent* CameraComp = ExecuteCameraActor->FindComponentByClass<UCameraComponent>();
			if (CameraComp)
			{
				CameraComp->bConstrainAspectRatio = false;
			}
		}
	}
}

void ARASPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ARASCharacterBase* LockOnTarget = CombatComponent->GetLockedOnTarget();
	if (LockOnTarget != nullptr && CombatComponent->GetCombatState() != EPlayerCombatState::Executing)
	{
		float CurDistanceToTarget = GetDistanceTo(LockOnTarget);
		if (CurDistanceToTarget > 2000.f)
		{
			LockOnTarget = nullptr;
			CombatComponent->PressTab();
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

	if (PlayerInputComponent)
	{
		PlayerInput->InitPlayerInputComponent(PlayerInputComponent);
	}
}

void ARASPlayer::Move(const FInputActionValue& Value)
{
	FVector2D CurrentMovementInput = Value.Get<FVector2D>();
	LastMoveInput = CurrentMovementInput;

	if (CombatComponent->GetCombatState() != EPlayerCombatState::Idle) return;

	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, CurrentMovementInput.X);
	AddMovementInput(RightDirection, CurrentMovementInput.Y);
}

void ARASPlayer::Look(const FInputActionValue& Value)
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Deathing || CombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if(CombatComponent->GetLockedOnTarget() == nullptr)
		AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ARASPlayer::LockOn()
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Deathing || CombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;


	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = true;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::LockOff()
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Deathing || CombatComponent->GetCombatState() == EPlayerCombatState::Executing) return;


	URASPlayerAnimInstance* MyAnimInstance = Cast<URASPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnimInstance == nullptr)
		return;

	bUseControllerRotationYaw = false;
	MyAnimInstance->SetLockOn(bUseControllerRotationYaw);
}

void ARASPlayer::SwitchToExecutionCamera(float BlendTime)
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Deathing) return;

	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(GetController());
	if (PlayerController)
	{
		OriginalCameraActor = PlayerController->GetViewTarget();
		PlayerController->SetViewTargetWithBlend(ExecuteCameraActor, BlendTime);
	}

}

void ARASPlayer::SwitchBackToOriginalCamera(float BlendTime)
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Deathing) return;

	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(OriginalCameraActor, BlendTime);
		OriginalCameraActor = nullptr;
	}
}

void ARASPlayer::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	if (CombatComponent->GetCombatState() == EPlayerCombatState::Rolling || CombatComponent->GetCombatState() == EPlayerCombatState::Armoring || CombatComponent->GetCombatState() == EPlayerCombatState::Deathing)
		return;
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	CombatComponent->HitFromActor(InFrom, InDamage, InStaminaDamage);
}

void ARASPlayer::Death()
{
	CombatComponent->SetCombatState(EPlayerCombatState::Deathing);

	Super::Death();

	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
		{
			// 플레이어 사망 UI 띄움, UI에서 다시하기 버튼 클릭시  게임모드에서 플레이어를 저장된 위치로 리스폰
		}, 
		4.f, false);
}
