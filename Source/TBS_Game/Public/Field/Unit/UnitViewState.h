#pragma once

UENUM(BlueprintType)
enum class EUnitViewState : uint8
{
	Initialize = 0				UMETA(DisplayName = "Initialize"),
	Preview = 1					UMETA(DisplayName = "Preview"),
	Assembling = 2				UMETA(DisplayName = "Assembling"),
	Idle = 3					UMETA(DisplayName = "Idle"),
	Count = 4					UMETA(Hidden)
};
