// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Character/Player/RASPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/Player/RASCombatComponent.h"


// Sets default values for this component's properties
URASInputComponent::URASInputComponent()
{
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
}

void URASInputComponent::InitPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandleRoll);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandleMove);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandleLook);
	EnhancedInput->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandlePressTab);
	EnhancedInput->BindAction(FAttackAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandlePressF);
	EnhancedInput->BindAction(LeftAttackAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandlePressComboAction);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Started, this, &URASInputComponent::HandlePressShift);
	EnhancedInput->BindAction(ShiftAttackAction, ETriggerEvent::Completed, this, &URASInputComponent::HandlePressShiftEnd);
	EnhancedInput->BindAction(QAttackAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandlePressQ);
	EnhancedInput->BindAction(EAttackAction, ETriggerEvent::Triggered, this, &URASInputComponent::HandlePressE);
	EnhancedInput->BindAction(ParryingAction, ETriggerEvent::Started, this, &URASInputComponent::HandlePressRightClick);
	EnhancedInput->BindAction(ParryingAction, ETriggerEvent::Completed, this, &URASInputComponent::HandlePressRightClickEnd);
}

void URASInputComponent::BeginPlay()
{
	Super::BeginPlay();

	ARASPlayer* Player = GetOwner<ARASPlayer>();

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController)
	{
		Player->EnableInput(PlayerController);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void URASInputComponent::HandleMove(const FInputActionValue& Value)
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());
	if (OwnerPlayer)
	{
		OwnerPlayer->Move(Value);
	}
}

void URASInputComponent::HandleLook(const FInputActionValue& Value)
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());
	if (OwnerPlayer)
	{
		OwnerPlayer->Look(Value);
	}
}

void URASInputComponent::HandleRoll(const FInputActionValue& Value)
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->Roll(Value);
	}
}

void URASInputComponent::HandlePressTab()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressTab();
	}
}

void URASInputComponent::HandlePressComboAction()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressComboAction();
	}
}

void URASInputComponent::HandlePressShift()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressShift();
	}
}

void URASInputComponent::HandlePressShiftEnd()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressShiftEnd();
	}
}

void URASInputComponent::HandlePressF()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressF();
	}
}

void URASInputComponent::HandlePressQ()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressQ();
	}
}

void URASInputComponent::HandlePressE()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressE();
	}
}

void URASInputComponent::HandlePressRightClick()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressRightClick();
	}
}

void URASInputComponent::HandlePressRightClickEnd()
{
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());

	if (URASCombatComponent* CombatComponent = OwnerPlayer->GetCombatComponent())
	{
		CombatComponent->PressRightClickEnd();
	}
}
