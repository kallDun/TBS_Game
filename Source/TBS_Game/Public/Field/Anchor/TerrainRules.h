#pragma once
#include "SingleTerrainRule.h"
#include "TerrainDepthRulesType.h"
#include "Field/Cell/TerrainType.h"
#include "TerrainRules.generated.h"

USTRUCT(BlueprintType)
struct TBS_GAME_API FTerrainRules
{
	GENERATED_BODY()

public:
	FTerrainRules()
	{
		Rules.Add(FSingleTerrainRule(ETerrainType::Grass, true, 0, 0));
		Rules.Add(FSingleTerrainRule(ETerrainType::Rocks, true, -2, 0));
		Rules.Add(FSingleTerrainRule(ETerrainType::Coast, true, 0, -1));
		Rules.Add(FSingleTerrainRule(ETerrainType::Mountains, false, 0, 0));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSingleTerrainRule> Rules = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrainDepthRulesType DepthRulesType = ETerrainDepthRulesType::CannotBuildWithDepth;

// functions
	FSingleTerrainRule GetTerrainRule(const ETerrainType TerrainType) const;
	
};
