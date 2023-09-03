#pragma once

UENUM(BlueprintType)
enum class EAnchorRadiusType : uint8
{
	Fixed = 0							UMETA(DisplayName = "Fixed"),
	CurrentBuildingProperty = 1			UMETA(DisplayName = "Current Building Property"),
	AnchorBuildingProperty = 2			UMETA(DisplayName = "Anchor's Building Property"),
};
