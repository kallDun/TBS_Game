#pragma once

UENUM(BlueprintType)
enum class ETerrainDepthRulesType : uint8
{
	CannotBuildWithDepth = 0		UMETA(DisplayName = "Cannot build with depth"),
	CanBuildWithDepth = 1			UMETA(DisplayName = "Can build with depth"),
	NeedDepthToBuild = 2			UMETA(DisplayName = "Need depth to build"),
};
