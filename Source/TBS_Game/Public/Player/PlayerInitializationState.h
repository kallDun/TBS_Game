#pragma once

UENUM(BlueprintType)
enum class EPlayerInitializationState : uint8
{
	NotInitialized = 0			UMETA(DisplayName = "Not Initialized"),
	Loaded = 1					UMETA(DisplayName = "Loaded"),
	Initialized = 2				UMETA(DisplayName = "Initialized"),
	
	Count = 3					UMETA(Hidden)
};