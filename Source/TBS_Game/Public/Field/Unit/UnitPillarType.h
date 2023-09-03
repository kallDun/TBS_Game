#pragma once

UENUM(BlueprintType)
enum class EUnitPillarType : uint8
{
	Warrior = 0			UMETA(DisplayName = "Army"),
	Saint = 1			UMETA(DisplayName = "Religion"),

	Count = 2			UMETA(Hidden)
};