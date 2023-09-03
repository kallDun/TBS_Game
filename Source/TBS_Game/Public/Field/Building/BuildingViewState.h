#pragma once

UENUM(BlueprintType)
enum class EBuildingViewState : uint8
{
	Initialize = 0				UMETA(DisplayName = "Initialize"),
	Preview = 1					UMETA(DisplayName = "Preview"),
	Building = 2				UMETA(DisplayName = "Building"),
	Upgrading = 3				UMETA(DisplayName = "Upgrading"),
	Ready = 4					UMETA(DisplayName = "Ready"),
	Count = 5					UMETA(Hidden)
};
