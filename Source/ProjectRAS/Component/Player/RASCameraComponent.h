// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RASCameraComponent.generated.h"
/**
 * URASCameraComponent 클래스
 * - 카메라 관련 기능을 담당합니다.
 * - 플레이어 카메라, 특정 액션 시 사용하는 카메라 등을 포함합니다.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URASCameraComponent();
	
	void InitCamera();
	void LockOnCamera(class ARASCharacterBase* InTarget, float DeltaTime);
	void SwitchToExecutionCamera(float BlendTime);
	void SwitchBackToOriginalCamera(float BlendTime);

protected:
	// - ExecuteCameraActor: 특정 액션 시 사용하는 카메라 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACameraActor> ExecuteCameraActor1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ACameraActor> ExecuteCameraActor2;

	// - OriginalCameraActor: 원래 사용하던 카메라 액터를 저장 (전환 후 복구용)
	TObjectPtr<class AActor> OriginalCameraActor;

};
