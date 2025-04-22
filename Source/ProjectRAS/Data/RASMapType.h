#pragma once

#include "CoreMinimal.h"
#include "RASMapType.generated.h"

UENUM(BlueprintType)
enum class ERASMapType : uint8
{
	None,
	Room,
	Corridor,
};

UENUM(BlueprintType)
enum class ERASRoomType : uint8
{
	None,
	Normal,
	Start,
	Boss,
	NPC,
};

UENUM(BlueprintType)
enum class ERASCorridorType : uint8
{
	None,
	Straight,
	BentRight,
	BentLeft,
};