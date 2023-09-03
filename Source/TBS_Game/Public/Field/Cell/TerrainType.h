#pragma once

UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	Grass = 0			UMETA(DisplayName = "Grass"),
	Rocks = 1			UMETA(DisplayName = "Rocks"),
	Mountains = 2		UMETA(DisplayName = "Mountains"),
	Coast = 3			UMETA(DisplayName = "Coast"),
	
	Count = 4			UMETA(Hidden)
};