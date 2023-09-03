#pragma once

UENUM(BlueprintType)
enum class EHeroState : uint8
{
    Blocked = 0				UMETA(DisplayName = "Blocked"),
    Initialized = 1				UMETA(DisplayName = "Initialized"),
	Ready = 2					UMETA(DisplayName = "Ready"),
								
	Count = 3					UMETA(Hidden)
};
