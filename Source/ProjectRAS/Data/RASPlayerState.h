#pragma once

#include "CoreMinimal.h"
#include "RASPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Parrying    UMETA(DisplayName = "Parrying"),
    Breaking    UMETA(DisplayName = "Breaking"),
    Rolling     UMETA(DisplayName = "Rolling"),
    Skilling    UMETA(DisplayName = "Skilling"),
    Armoring    UMETA(DisplayName = "Armoring"),
    Deathing    UMETA(DisplayName = "Deathing"),
};
