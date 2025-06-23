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
		{
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			FVector SpawnOffset(0.f, 218.7f, 56.3f);
			FVector SpawnLocation = OwnerLocation + SpawnOffset;
			FRotator SpawnRotation(-5.f, -50.f, 0.f);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ExecuteCameraActor1 = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			if (ExecuteCameraActor1)
			{
				ExecuteCameraActor1->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				if (UCameraComponent* CameraComp = ExecuteCameraActor1->FindComponentByClass<UCameraComponent>())
				{
					CameraComp->bConstrainAspectRatio = false;
				}
			}
		}
		{
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			FVector SpawnOffset(0.f, -218.7f, 56.3f);
			FVector SpawnLocation = OwnerLocation + SpawnOffset;
			FRotator SpawnRotation(-5.f, 50.f, 0.f);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			ExecuteCameraActor2 = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			if (ExecuteCameraActor2)
			{
				ExecuteCameraActor2->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
				if (UCameraComponent* CameraComp = ExecuteCameraActor2->FindComponentByClass<UCameraComponent>())
				{
					CameraComp->bConstrainAspectRatio = false;
				}
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
	ARASPlayer* OwnerPlayer = Cast<ARASPlayer>(GetOwner());
	if (!OwnerPlayer)
	{
		return;
	}

	ARASPlayerController* PlayerController = Cast<ARASPlayerController>(OwnerPlayer->GetController());
	if (!PlayerController)
	{
		return;
	}

	OriginalCameraActor = PlayerController->GetViewTarget();

	FVector StartLocation = OwnerPlayer->GetActorLocation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerPlayer);
	CollisionParams.AddIgnoredActor(ExecuteCameraActor1);
	CollisionParams.AddIgnoredActor(ExecuteCameraActor2);

	auto IsLineOfSightClear = [&](AActor* TargetCamera) -> bool
		{
			FHitResult HitResult;
			return !GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				TargetCamera->GetActorLocation(),
				ECollisionChannel::ECC_Visibility,
				CollisionParams
			);
		};

	// 시야가 트여 있는 카메라로 전환
	if (IsLineOfSightClear(ExecuteCameraActor1))
	{
		PlayerController->SetViewTargetWithBlend(ExecuteCameraActor1, BlendTime);
	}
	else if (IsLineOfSightClear(ExecuteCameraActor2))
	{
		PlayerController->SetViewTargetWithBlend(ExecuteCameraActor2, BlendTime);
	}
	else
	{
		OriginalCameraActor = nullptr;
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

