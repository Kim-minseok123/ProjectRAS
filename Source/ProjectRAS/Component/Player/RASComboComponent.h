#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RASComboAttackData.h"
#include "RASComboComponent.generated.h"

/**
 * URASComboComponent
 * - 플레이어의 콤보 공격 로직을 관리하는 컴포넌트입니다.
 * - 콤보 시작, 타이머를 통한 상태 전이 및 종료 등 콤보 관련 동작을 수행합니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRAS_API URASComboComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	URASComboComponent();
	
	// 콤보 공격 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	// Data Asset에서 불러온 콤보 상태 정보를 저장하는 맵
	UPROPERTY(VisibleAnywhere, Category = "Combo")
	TMap<FName, FComboState> ComboStateMap;

	// 에디터에서 할당할 콤보 데이터 에셋
	UPROPERTY(EditAnywhere, Category = "Combo")
	TObjectPtr<class URASComboAttackData> ComboDataAsset;

	// 콤보 액션 버튼이 눌렸을 때 호출되는 함수
	UFUNCTION(BlueprintCallable)
	void PressComboAction(EAttackType InAttackType);

	// 현재 콤보를 종료하고 상태를 초기화할 때 호출하는 함수
	void EndCombo(bool InbSetTimer = true, float InSeconds = 0.7f);

protected:
	virtual void BeginPlay() override;

	// 콤보 시작: 콤보 상태를 초기 상태("A")로 설정하고 관련 로직 실행
	void StartCombo();

	// 타이머 만료 시 호출되는 함수
	// - 입력이 있었으면 상태 전이, 없으면 콤보 종료 처리
	void ComboTimerExpired();

	// 현재 상태에 맞는 콤보 타이머를 설정하는 함수
	void SetComboTimer();

	// 현재 콤보 상태와 입력(InAttackType)에 따른 다음 상태를 결정하는 함수
	// OutNextState에 결과를 저장하고, 성공 시 true를 반환
	bool GetNextState(EAttackType InAttackType, FName& OutNextState);

private:
	// 현재 콤보 상태를 나타내는 이름 (예: "A", "B", "C" 등)
	FName CurrentState;

	// 콤보 진행 중 상태 전환을 위한 타이머 핸들
	FTimerHandle ComboTimerHandle;

	// 콤보 입력이 발생했는지 여부
	bool bHasPendingInput;

	// 추가 입력을 받을 수 있는 상태인지 여부
	bool bCanAcceptInput;

	// 콤보 리셋을 위한 타이머 핸들
	FTimerHandle ComboResetHandle;

	// 대기 중인 공격 타입 (입력 보류 상태에서 저장)
	EAttackType PendingAttackType;
};