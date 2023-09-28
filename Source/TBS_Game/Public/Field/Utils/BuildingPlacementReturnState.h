#pragma once

UENUM(BlueprintType)
enum class EBuildingPlacementReturnState : uint8
{
	Unknown = 255							UMETA(DisplayName = "Unknown"),
	NotPlayerMove = 0						UMETA(DisplayName = "Not Player Move"),
	RequirementsNotMet = 1					UMETA(DisplayName = "Requirement Not Met"),
	LocationRequirementsNotMet = 2 			UMETA(DisplayName = "Location Requirements Not Met"),
	LocationNotAvailableForPlayer = 3		UMETA(DisplayName = "Location Not Available For Player"),
	LocationOccupied = 4					UMETA(DisplayName = "Location Occupied"),
	NotEnoughAreaToBuild = 5 				UMETA(DisplayName = "Not Enough Area To Build"),
	CannotExpandArea = 6					UMETA(DisplayName = "Cannot Expand Area"),
	NotEnoughMoves = 7						UMETA(DisplayName = "Not Enough Moves"),
	Succeeded = 8							UMETA(DisplayName = "Succeeded")
};
