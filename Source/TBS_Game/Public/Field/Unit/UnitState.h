#pragma once

UENUM(BlueprintType)
enum class EUnitState : uint8
{
    Initialized = 0				UMETA(DisplayName = "Initialized"),
	Ready = 1					UMETA(DisplayName = "Ready"),
	Rest = 2					UMETA(DisplayName = "Rest"),
	Destroyed = 3			    UMETA(DisplayName = "Destroyed"),
								
	Count = 4					UMETA(Hidden)
};
