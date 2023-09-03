#pragma once

UENUM(BlueprintType)
enum class EAnchorPointType : uint8
{
	Fixed = 0					UMETA(DisplayName = "Fixed"),
	PlayerInitialPosition = 1	UMETA(DisplayName = "Player Initial Position"),
	BehaviourComponent = 2		UMETA(DisplayName = "Behaviour Component"),
	BuildingName = 3			UMETA(DisplayName = "Building Name"),
};
