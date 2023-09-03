#pragma once

UENUM(BlueprintType)
enum class EUpgradingState : uint8
{
	Initialized = 0				UMETA(DisplayName = "Initialized"),
	Building = 1				UMETA(DisplayName = "Building"),
	Assembling = 2				UMETA(DisplayName = "Assembling"),
	Ready = 3					UMETA(DisplayName = "Ready"),
								
	Count = 4					UMETA(Hidden)
};
