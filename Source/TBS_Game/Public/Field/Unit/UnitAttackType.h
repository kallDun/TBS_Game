#pragma once

UENUM(BlueprintType)
enum class EUnitAttackType : uint8
{
	Melee = 0			UMETA(DisplayName = "Melee"),
	Range = 1			UMETA(DisplayName = "Range"),
	Siege = 2			UMETA(DisplayName = "Siege"),
	Spy	= 3				UMETA(DisplayName = "Spy"),

	Count = 4			UMETA(Hidden)
};