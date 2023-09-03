#pragma once
#include "Field/HexagonLocation.h"
#include "SingleAnchorData.generated.h"


USTRUCT(BlueprintType)
struct TBS_GAME_API FSingleAnchorData
{
	GENERATED_BODY()
	
public:
	FSingleAnchorData() = default;
	FSingleAnchorData(const FHexagonLocation Location, const int Radius, const int MaximumDebuffLevelToBuild) :
		Location(Location), Radius(Radius), MaximumDebuffLevelToBuild(MaximumDebuffLevelToBuild) {}

	UPROPERTY(BlueprintReadOnly)
	FHexagonLocation Location;

	UPROPERTY(BlueprintReadOnly)
	int Radius;

	UPROPERTY(BlueprintReadOnly)
	int MaximumDebuffLevelToBuild;
};