#pragma once

UENUM(BlueprintType)
enum class EUnitPlacementReturnState : uint8
{
	Unknown = 255							UMETA(DisplayName = "Unknown"),
	NotPlayerMove = 0						UMETA(DisplayName = "Not Player Move"),
	RequirementsNotMatch = 1				UMETA(DisplayName = "Requirement Not Match"),
	LocationRequirementsNotMatch = 2 		UMETA(DisplayName = "Location Requirements Not Match"),
	BuildingRequirementsNotMatch = 3 		UMETA(DisplayName = "Building Requirements Not Match"),
	LocationNotAvailableForPlayer = 4		UMETA(DisplayName = "Location Not Available For Player"),
	LocationOccupied = 5					UMETA(DisplayName = "Location Occupied"),
	NotEnoughAreaToBuild = 6 				UMETA(DisplayName = "Not Enough Area To Build"),
	CannotExpandArea = 7					UMETA(DisplayName = "Cannot Expand Area"),
	NotEnoughMoves = 8						UMETA(DisplayName = "Not Enough Moves"),
	ReachedLimit = 9						UMETA(DisplayName = "Limit has reached"),
	Succeeded = 10							UMETA(DisplayName = "Succeeded")
};