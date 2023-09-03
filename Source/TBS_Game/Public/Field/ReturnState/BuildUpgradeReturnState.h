#pragma once

UENUM(BlueprintType)
enum class EBuildUpgradeReturnState : uint8
{
	Unknown = 255							UMETA(DisplayName = "Unknown"),
	NotPlayerMove = 0						UMETA(DisplayName = "Not Player Move"),
	RequirementsNotMet = 1					UMETA(DisplayName = "Requirement Not Met"),
	LocationOccupied = 2					UMETA(DisplayName = "Location Occupied"),
	AreaNotDefined = 3						UMETA(DisplayName = "Area Not Defined"),
	IncorrectState = 4						UMETA(DisplayName = "Incorrect State"),
	NoUpgradeAvailable = 5					UMETA(DisplayName = "No Upgrade Available"),
	NotEnoughMoves = 6						UMETA(DisplayName = "Not Enough Moves"),
	Succeeded = 7							UMETA(DisplayName = "Succeeded")
};
