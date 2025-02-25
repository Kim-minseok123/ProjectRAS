// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RASComboAttackData.generated.h"
/** 공격 입력 타입 (에디터와 블루프린트에서 사용하기 위해 BlueprintType으로 지정) */
UENUM(BlueprintType)
enum class EAttackType : uint8 {
    LeftClick UMETA(DisplayName = "Left Click"),
    Shift     UMETA(DisplayName = "Shift"),
    F         UMETA(DisplayName = "F")
};

/** 하나의 전이 정보를 나타내는 구조체 */
USTRUCT(BlueprintType)
struct FComboTransition
{
    GENERATED_BODY()

    // 입력한 공격 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    EAttackType AttackType;

    // 이 입력을 받았을 때 전이할 다음 상태 이름
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    FName NextState;
};

/** 하나의 콤보 상태 정보를 나타내는 구조체 */
USTRUCT(BlueprintType)
struct FComboState
{
    GENERATED_BODY()

    // 상태 이름 (예: "A", "B", "C" 등)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    FName StateName;

    // 이 상태에서 입력을 받을 수 있는 유효 시간 (초)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    float EffectiveTime;

    // 상태 전이를 위한 입력과 다음 상태 정보 목록
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    TArray<FComboTransition> Transitions;

    // 현재 상태가 콤보의 마지막 공격 상태인지 여부
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    bool bIsLast;
};
/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    URASComboAttackData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
    TArray<FComboState> ComboStates;
};
