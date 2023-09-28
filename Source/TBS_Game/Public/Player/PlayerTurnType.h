#pragma once

UENUM(BlueprintType)
enum class EPlayerTurnType : uint8
{
	Waiting = 0					UMETA(DisplayName = "Waiting"),
	UnitsMove = 1				UMETA(DisplayName = "Units Move"),
	WinCheck = 2				UMETA(DisplayName = "Win Check"),
	UnitsAssembling = 3			UMETA(DisplayName = "Units Assembling"),
	BuildingsAssembling = 4		UMETA(DisplayName = "Buildings Assembling"),
	BuildingsPreMove = 5		UMETA(DisplayName = "Buildings Pre Move"),
	PlayerMove = 6				UMETA(DisplayName = "Player Move"),
	BuildingsPostMove = 7		UMETA(DisplayName = "Buildings Post Move"),
								
	Count = 8					UMETA(Hidden)
};
