#pragma once
#include "SingleTerrainRule.generated.h"

enum class ETerrainType : uint8;

USTRUCT(BlueprintType)
struct TBS_GAME_API FSingleTerrainRule
{
	GENERATED_BODY()

public:
	FSingleTerrainRule() = default;
	FSingleTerrainRule(const ETerrainType TerrainType, const bool bCanBuild, const int BuildingRadiusAddition, const int ImproveLevelAddition)
		: TerrainType(TerrainType), bCanBuild(bCanBuild), BuildingRadiusAddition(BuildingRadiusAddition), ImproveLevelAddition(ImproveLevelAddition) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrainType TerrainType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBuild = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BuildingRadiusAddition = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ImproveLevelAddition = 0;
};
