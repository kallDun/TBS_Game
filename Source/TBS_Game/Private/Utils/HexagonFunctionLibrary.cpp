#include "Utils/HexagonFunctionLibrary.h"

#include "Field/FieldActor.h"
#include "Field/HexagonLocation.h"
#include "Kismet/KismetMathLibrary.h"


FVector UHexagonFunctionLibrary::GetWorldLocationFromHexagon(const FHexagonLocation& HexagonLocation,
	const float HexagonSize, const FVector FieldCenter)
{
	const float Magic = UKismetMathLibrary::Sqrt(3.0f) / 2.0f;
	const float X_Size = HexagonSize;
	const float Y_Size = HexagonSize * Magic;
	FVector WorldLocation = FVector(FieldCenter.X + X_Size * HexagonLocation.X, FieldCenter.Y + Y_Size * HexagonLocation.Y, FieldCenter.Z);
	if (HexagonLocation.Y % 2 != 0)
	{
		WorldLocation.X += X_Size / 2.0f;
	}
	return WorldLocation;
}

int UHexagonFunctionLibrary::GetDistanceBetweenHexagons(const FHexagonLocation& Start,
	const FHexagonLocation& Dest)
{
	return FMath::Max3(
		FMath::Abs(Dest.Y - Start.Y),
		FMath::Abs(FMath::CeilToInt(Dest.Y / -2.0f) + Dest.X - FMath::CeilToInt(Start.Y / -2.0f) - Start.X),
		FMath::Abs(-Dest.Y - FMath::CeilToInt(Dest.Y / -2.0f) - Dest.X + Start.Y + FMath::CeilToInt(Start.Y / -2.0f) + Start.X));
}

int UHexagonFunctionLibrary::GetMinDistanceFromHexagons(const FHexagonLocation& Start,
	const TArray<FHexagonLocation>& Destinations)
{
	if (Destinations.Num() > 0)
	{
		int MinDistance = GetDistanceBetweenHexagons(Start, Destinations[0]);
		for (int i = 1; i < Destinations.Num(); ++i)
		{
			const int Distance = GetDistanceBetweenHexagons(Start, Destinations[i]);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
			}
		}
		return MinDistance;
	}
	return -1;
}

TArray<FHexagonLocation> UHexagonFunctionLibrary::ToHexagonsArray(const TArray<AFieldActor*> FieldActors)
{
	TArray<FHexagonLocation> Hexagons = {};
	for (const AFieldActor* FieldActor : FieldActors)
	{
		Hexagons.Add(FieldActor->GetLocation());
	}
	return Hexagons;
}

TArray<FHexagonLocation> UHexagonFunctionLibrary::CombineHexagonArrays(const TArray<FHexagonLocation>& First,
	const TArray<FHexagonLocation>& Second)
{
	TArray<FHexagonLocation> Hexagons = First;
	for (const FHexagonLocation& Hexagon : Second)
	{
		Hexagons.AddUnique(Hexagon);
	}
	return Hexagons;
}
