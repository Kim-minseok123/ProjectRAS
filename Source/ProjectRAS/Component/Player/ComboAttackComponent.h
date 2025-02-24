#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ComboAttackData.h"
#include "ComboAttackComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTRAS_API UComboAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UComboAttackComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UFUNCTION(BlueprintCallable)
	void PressComboAction(EAttackType InAttackType);

    // Data Asset에서 불러온 콤보 상태 정보를 저장하는 맵
    UPROPERTY(VisibleAnywhere, Category = "Combo")
    TMap<FName, FComboState> ComboStateMap;

    // 에디터에서 할당할 Data Asset
    UPROPERTY(EditAnywhere, Category = "Combo")
    TObjectPtr<class UComboAttackData> ComboDataAsset;

protected:
	virtual void BeginPlay() override;

	// 콤보 시작 (상태 "A"로 초기화)
	void StartCombo();

	// 타이머 만료 시 호출 – 입력이 있었으면 상태 전이, 없으면 콤보 종료
	void ComboTimerExpired();

public:
	// 현재 콤보를 종료하고 상태를 초기화
	void EndCombo(bool InbSetTiemr = true);
protected:
	// 현재 상태에 맞는 타이머 설정
	void SetComboTimer();

	// 현재 상태와 입력(InAttackType)에 따른 다음 상태를 lookup
	bool GetNextState(EAttackType InAttackType, FName& OutNextState);

private:
	FName CurrentState;
	FTimerHandle ComboTimerHandle;
	bool bHasPendingInput;
	bool bCanAcceptInput;
	FTimerHandle ComboResetHandle;
	EAttackType PendingAttackType;
};
