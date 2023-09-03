#pragma once

UENUM(BlueprintType)
enum class EHeroPillarType : uint8
{
	Warrior = 0			UMETA(DisplayName = "Army"),
	Saint = 1			UMETA(DisplayName = "Religion"),
	Economic = 2		UMETA(DisplayName = "Economic"),

	Count = 3			UMETA(Hidden)
};