#pragma once

UENUM(BlueprintType)
enum class EPlayerTurnType : uint8
{
	Waiting = 0					UMETA(DisplayName = "Waiting"),
	UnitsMove = 1				UMETA(DisplayName = "Units Move"),
	WinCheck = 2				UMETA(DisplayName = "Win Check"),
	BuildingsAssembling = 3		UMETA(DisplayName = "Buildings Assembling"),
	BuildingsPreMove = 4		UMETA(DisplayName = "Buildings Pre Move"),
	PlayerMove = 5				UMETA(DisplayName = "Player Move"),
	BuildingsPostMove = 6		UMETA(DisplayName = "Buildings Post Move"),
								
	Count = 7					UMETA(Hidden)
};
