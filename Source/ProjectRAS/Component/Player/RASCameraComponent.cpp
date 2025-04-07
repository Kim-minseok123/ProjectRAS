// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/RASCameraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraActor.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASCombatComponent.h"
#include "Controller/Player/RASPlayerController.h"

URASCameraComponent::URASCameraComponent()
{
	
}

void URASCameraComponent::InitCamera()
{
	if (GetOwner()->GetWorld())
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector SpawnOffset(0.f, 218.7f, 56.3f);
		FVector SpawnLocation = OwnerLocation + SpawnOffset;
		FRotator SpawnRotation(-5.f, -50.f, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ExecuteCameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

		if (ExecuteCameraActor)
		{
			ExecuteCameraActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
			if (UCameraComponent* CameraComp = ExecuteCameraActor->FindComponentByClass<UCameraComponent>())
			{
				CameraComp->bConstrainAspectRatio = false;
			}
		}
	}
}


void URASCameraComponent::LockOnCamera(class ARASCharacterBase* InTarget, float DeltaTime)
{
	if (InTarget != nullptr)
	{
		ARASPlayer* Player = GetOwner<ARASPlayer>();
		float CurDistanceToTarget = Player->GetDistanceTo(InTarget);
		if (CurDistanceToTarget > 2000.f)
		{
			InTarget = nullptr;
			Player->GetCombatComponent()->PressTab();
			return;
		}
		FRotator CurrentControlRot = Player->GetControlRotation();
		FVector PlayerLocation = Player->GetActorLocation();
		FVector TargetLocation = InTarget->GetActorLocation();

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

		FRotator NewRot = FMath::RInterpTo(CurrentControlRot, TargetRotation, DeltaTime, 5.f);

		FRotator FinalRotation(CurrentControlRot.Pitch, NewRot.Yaw, CurrentControlRot.Roll);
		Player->GetController()->SetControlRotation(FinalRotation);
	}
}

void URASCameraComponent::SwitchToExecutionCamera(float BlendTime)
{
	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(GetOwner<ARASPlayer>()->GetController());
	if (PlayerController)
	{
		OriginalCameraActor = PlayerController->GetViewTarget();
		PlayerController->SetViewTargetWithBlend(ExecuteCameraActor, BlendTime);
	}
}

void URASCameraComponent::SwitchBackToOriginalCamera(float BlendTime)
{
	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(GetOwner<ARASPlayer>()->GetController());
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(OriginalCameraActor, BlendTime);
		OriginalCameraActor = nullptr;
	}
}

