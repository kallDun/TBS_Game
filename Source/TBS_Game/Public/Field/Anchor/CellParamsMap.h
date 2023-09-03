#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "AnchorPoint.h"
#include "CellParameters.h"
#include "SingleAnchorData.h"
#include "TerrainRules.h"
#include "Field/Cell/Cell.h"
#include "CellParamsMap.generated.h"

class UCellParamsTwoDimArray;
class AGamePlayerController;
class ABuilding;
class AUnit;
class AFieldController;

UCLASS(BlueprintType)
class TBS_GAME_API UCellParamsMap : public UObject
{
	GENERATED_BODY()
	UCellParamsMap() = default;

public:
	UPROPERTY(BlueprintReadOnly)
	UCellParamsTwoDimArray* Cells;

public:
	UFUNCTION(BlueprintCallable)
	static UCellParamsMap* FromBuilding(const ABuilding* Building);

	UFUNCTION(BlueprintCallable)
	static UCellParamsMap* FromUnit(const AUnit* Unit);

	UFUNCTION(BlueprintCallable)
	static UCellParamsMap* Add(const UCellParamsMap* MapA, const UCellParamsMap* MapB);

private:
	UFUNCTION(BlueprintCallable)
	void Init(const ABuilding* Building, const FAnchorPoint& Anchor);

	UFUNCTION(BlueprintCallable)
	void InitUnit(const AUnit* Unit, const FAnchorPoint& Anchor);

public:
	UFUNCTION(BlueprintCallable)
	FCellParameters GetCell(FHexagonLocation Location) const;
	
private:
	UFUNCTION(BlueprintCallable)
	static UCellParamsMap* FromBuildingAnchor(const ABuilding* Building, const FAnchorPoint& Anchor);

	UFUNCTION(BlueprintCallable)
	static UCellParamsMap* FromUnitAnchor(const AUnit* Unit);
	
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* AddArrays(UCellParamsTwoDimArray* MapA, UCellParamsTwoDimArray* MapB);

	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* GetCellsMapForSingleAnchor(const ABuilding* Building, const FSingleAnchorData& SingleAnchor);

	UFUNCTION(BlueprintCallable)	
	static bool CheckTerrainRules(const ACell* Cell, const FTerrainRules& TerrainRules);
	
	UFUNCTION(BlueprintCallable)
	static int CalculateImprovementLevel(const ACell* Cell, const ABuilding* Building, FSingleAnchorData SingleAnchor, const int Distance);
};
