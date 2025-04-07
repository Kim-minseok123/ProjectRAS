#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "InputActionValue.h"
#include "Data/RASPlayerState.h"
#include "RASPlayer.generated.h"

/**
 * ARASPlayer 클래스
 * - 플레이어의 이동, 입력, 카메라 제어, UI, 전투 등을 담당합니다.
 */
UCLASS()
class PROJECTRAS_API ARASPlayer : public ARASCharacterBase
{
	GENERATED_BODY()

public:
	// 생성자
	ARASPlayer();

	// 오버라이드 함수들
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage) override;
	virtual void Death() override;

	// 입력 관련 함수들
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// 전투 및 기타 기능 관련 Getter
	class URASCombatComponent* GetCombatComponent() const { return PlayerCombatComponent; }
	class URASComboComponent* GetComboComponent() const { return PlayerComboAttack; }
	class URASPlayerAnimComponent* GetAnimComponent() { return PlayerAnimComponent; }
	class URASInputComponent* GetInputComponent() { return PlayerInput; }
	class URASUIComponent* GetUIComponent() { return PlayerUIComponent; }
	FVector2D GetLastMoveInput() const { return LastMoveInput; }

	// 타겟 Lock-On 애니메이션	 관련 함수들
	void LockOn();
	void LockOff();

	// 카메라 전환 관련 함수들
	void SwitchToExecutionCamera(float BlendTime);
	void SwitchBackToOriginalCamera(float BlendTime);

protected:
	// 입력 벡터를 저장 (최근 이동 입력)
	FVector2D LastMoveInput;

	// 전투 컴포넌트: 플레이어 전투 로직을 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASCombatComponent> PlayerCombatComponent;

	// 커스텀 입력 컴포넌트: 플레이어 입력 처리를 담당
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASInputComponent> PlayerInput;

	// 콤보 공격 컴포넌트: 연계 공격 로직 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASComboComponent> PlayerComboAttack;

	// 애님 컴포넌트: 플레이어 애니메이션 처리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASPlayerAnimComponent> PlayerAnimComponent;

	// UI 컴포넌트: HUD 및 기타 UI 관리를 담당
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASUIComponent> PlayerUIComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class URASCameraComponent> PlayerCameraComponent;

	// - CameraBoom: 카메라 부스터 역할 (스프링암)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	// - FollowCamera: 플레이어를 따라다니는 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// - HeadPoint: 머리 위치 등 카메라 정렬에 사용될 씬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> HeadPoint;
};