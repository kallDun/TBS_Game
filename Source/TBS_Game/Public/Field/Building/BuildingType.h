#pragma once

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Religious = 0			UMETA(DisplayName = "Religious"),
	Economic = 1			UMETA(DisplayName = "Economic"),
	Spy	= 2					UMETA(DisplayName = "Spy"),
	UnitProduction = 3		UMETA(DisplayName = "UnitProduction"),
	Terraform = 4			UMETA(DisplayName = "Terraform"),
	Military = 5			UMETA(DisplayName = "Military"),
	Defensive = 6			UMETA(DisplayName = "Defensive"),
	Fundamental	= 7			UMETA(DisplayName = "Fundamental"),

	Count = 8				UMETA(Hidden)
};