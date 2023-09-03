#pragma once
#include "CellParametersType.h"
#include "Field/HexagonLocation.h"
#include "CellParameters.generated.h"

USTRUCT(BlueprintType)
struct FCellParameters
{
	GENERATED_BODY()

public:
	FCellParameters() = default;
	
	FCellParameters(const FHexagonLocation InLocation, const ECellParametersType InParametersType,
		const int InPlayerOwnerNumber, const int InImproveLevel,
		const int MinDistanceToClosestBuildingView, const int MinDistanceToClosestAnchor)
		: Location(InLocation)
		, ParametersType(InParametersType)
		, PlayerOwnerNumber(InPlayerOwnerNumber)
		, ImproveLevel(InImproveLevel)
		, MinDistanceToClosestBuildingView(MinDistanceToClosestBuildingView)
		, MinDistanceToClosestAnchor(MinDistanceToClosestAnchor) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHexagonLocation Location;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECellParametersType ParametersType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlayerOwnerNumber;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ImproveLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinDistanceToClosestBuildingView;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MinDistanceToClosestAnchor;
};
