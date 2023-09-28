#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "AnchorPoint.h"
#include "SingleAnchorData.h"
#include "TerrainRules.h"
#include "Field/Cell/Cell.h"
#include "CellParamsMapGenerator.generated.h"

class UCellParamsTwoDimArray;
class AGamePlayerController;
class ABuilding;
class AUnit;
class AFieldController;

UCLASS(BlueprintType)
class TBS_GAME_API UCellParamsMapGenerator : public UObject
{
	GENERATED_BODY()
	UCellParamsMapGenerator() = default;

public:
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* New(const AFieldActorsHandler* Actor);

private:
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* InitFromAnchor(const AFieldActorsHandler* Actor, const FAnchorPoint& Anchor);
	
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* AddArrays(UCellParamsTwoDimArray* MapA, UCellParamsTwoDimArray* MapB);

	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* GetBuildingRelatedCellsMapForSingleAnchor(const ABuilding* Building, const FSingleAnchorData& SingleAnchor);
	
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* GetUnitRelatedCellsMapForSingleAnchor(const AUnit* Unit, const FSingleAnchorData& SingleAnchor);

	UFUNCTION(BlueprintCallable)	
	static bool CheckTerrainRules(const ACell* Cell, const FTerrainRules& TerrainRules);
	
	UFUNCTION(BlueprintCallable)
	static int CalculateImprovementLevel(const ACell* Cell, const AFieldActorsHandler* Actor, const FSingleAnchorData SingleAnchor, const int Distance);
};
