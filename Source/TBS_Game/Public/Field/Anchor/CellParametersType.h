#pragma once

UENUM(BlueprintType)
enum class ECellParametersType : uint8
{
	NotBelongs = 0							UMETA(DisplayName = "Not Belongs"),
	IsOccupied = 1							UMETA(DisplayName = "Is Occupied"),
	ForbiddenByTerrain = 2					UMETA(DisplayName = "Forbidden by Terrain"),
	ForbiddenByDebuffLevel = 3				UMETA(DisplayName = "Forbidden by Debuff Level"),
	Free = 4								UMETA(DisplayName = "Free"),
};
