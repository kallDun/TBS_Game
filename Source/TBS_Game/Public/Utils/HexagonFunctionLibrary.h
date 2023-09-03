#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "HexagonFunctionLibrary.generated.h"

class AFieldActor;
struct FHexagonLocation;

UCLASS()
class TBS_GAME_API UHexagonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FVector GetWorldLocationFromHexagon(const FHexagonLocation& HexagonLocation, const float HexagonSize, const FVector FieldCenter);
	
	UFUNCTION(BlueprintCallable)
	static int GetDistanceBetweenHexagons(const FHexagonLocation& Start, const FHexagonLocation& Dest);

	UFUNCTION(BlueprintCallable)
	static int GetMinDistanceFromHexagons(const FHexagonLocation& Start, const TArray<FHexagonLocation>& Destinations);

	UFUNCTION(BlueprintCallable)
	static TArray<FHexagonLocation> ToHexagonsArray(const TArray<AFieldActor*> FieldActors);

	UFUNCTION(BlueprintCallable)
	static TArray<FHexagonLocation> CombineHexagonArrays(const TArray<FHexagonLocation>& First, const TArray<FHexagonLocation>& Second);
};
