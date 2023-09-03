#pragma once
#include "HexagonLocation.generated.h"

USTRUCT(BlueprintType)
struct FHexagonLocation
{
	GENERATED_BODY()

public:
	FHexagonLocation() : X(0), Y(0) {}
	FHexagonLocation(const int X, const int Y) : X(X), Y(Y) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Y;
	
	bool operator==(const FHexagonLocation& Location) const
	{
		return X == Location.X && Y == Location.Y;
	}
};
