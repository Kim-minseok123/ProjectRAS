#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComboAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	LeftClick UMETA(DisplayName = "LeftClick"),
	Shift     UMETA(DisplayName = "Shift"),
	F         UMETA(DisplayName = "F")
};

// 콤보 상태 정보를 담는 구조체
USTRUCT(BlueprintType)
struct FComboState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	FName StateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	float EffectiveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	uint8 bIsLast : 1;

	// 해당 상태에서 입력에 따른 다음 상태 매핑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
	TMap<EAttackType, FName> Transitions;
};

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

protected:
	virtual void BeginPlay() override;

	// 콤보 시작 (상태 "A"로 초기화)
	void StartCombo();

	// 타이머 만료 시 호출 – 입력이 있었으면 상태 전이, 없으면 콤보 종료
	void ComboTimerExpired();

	// 현재 콤보를 종료하고 상태를 초기화
	void EndCombo();

	// 현재 상태에 맞는 타이머 설정
	void SetComboTimer();

	// 현재 상태와 입력(InAttackType)에 따른 다음 상태를 lookup
	bool GetNextState(EAttackType InAttackType, FName& OutNextState);

	// 콤보 상태 머신 데이터를 초기화
	void InitializeComboStates();

private:
	FName CurrentState;
	FTimerHandle ComboTimerHandle;
	bool bHasPendingInput;
	EAttackType PendingAttackType;
	TMap<FName, FComboState> ComboStateMap;
};
