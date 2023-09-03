#pragma once

UENUM(BlueprintType)
enum class EFieldControllerState : uint8
{
	NotInitialized = 0			UMETA(DisplayName = "Not Initialized"),
	WaitingForPlayers = 1		UMETA(DisplayName = "Waiting For Players"),
	PlayersInitialized = 2		UMETA(DisplayName = "Players Initialized"),
	GeneratingField = 3			UMETA(DisplayName = "Generating Field"),
	GameStarted = 4				UMETA(DisplayName = "Game Started"),
	Count = 5					UMETA(Hidden)
};
