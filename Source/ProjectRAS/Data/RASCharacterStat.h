#pragma once

#include "CoreMinimal.h"
#include "RASCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FRASCharacterStats
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxHP = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float HP = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MaxStamina = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float Stamina = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float AttackPower = 10.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    int32 MaxAttackNumber;
};
