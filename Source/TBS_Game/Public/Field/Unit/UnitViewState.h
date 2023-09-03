#pragma once

UENUM(BlueprintType)
enum class EUnitViewState : uint8
{
	Initialize = 0				UMETA(DisplayName = "Initialize"),
	Preview = 1					UMETA(DisplayName = "Preview"),
	Building = 2				UMETA(DisplayName = "Building"),
	Rest = 3					UMETA(DisplayName = "Rest"),
	Count = 4					UMETA(Hidden)
};
