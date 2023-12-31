#include "Field/Anchor/CellParamsMap.h"
#include "Field/Building/Building.h"
#include "Field/Cell/Cell.h"
#include "Field/Controller/FieldController.h"
#include "Player/GamePlayerController.h"
#include "Utils/FieldActorsFunctionLibrary.h"
#include "Field/FieldActor.h"
#include "Field/Building/BuildingView.h"
#include "Utils/HexagonFunctionLibrary.h"
#include "Utils/TwoDimArray/CellParamsTwoDimArray.h"
#include "Utils/TwoDimArray/CellTwoDimArray.h"


UCellParamsMap* UCellParamsMap::FromBuilding(const ABuilding* Building)
{
	UCellParamsMap* Map = NewObject<UCellParamsMap>();
	for (const FAnchorPoint Anchor : Building->AnchorPoints)
	{
		Map = Add(Map, FromBuildingAnchor(Building, Anchor));
	}
	return Map;
}

UCellParamsMap* UCellParamsMap::FromUnit(const AUnit* Unit)
{
	UCellParamsMap* Map = NewObject<UCellParamsMap>();
	return Map;
}

UCellParamsMap* UCellParamsMap::FromBuildingAnchor(const ABuilding* Building, const FAnchorPoint& Anchor)
{
	UCellParamsMap* Map = NewObject<UCellParamsMap>();
	Map->Init(Building, Anchor);
	return Map;
}

UCellParamsMap* UCellParamsMap::FromUnitAnchor(const AUnit* Unit)
{
	UCellParamsMap* Map = NewObject<UCellParamsMap>();
	return Map;
}

UCellParamsMap* UCellParamsMap::Add(const UCellParamsMap* MapA, const UCellParamsMap* MapB)
{
	UCellParamsMap* Map = NewObject<UCellParamsMap>();
	Map->Cells = AddArrays(MapA->Cells, MapB->Cells);
	return Map;
}

UCellParamsTwoDimArray* UCellParamsMap::AddArrays(UCellParamsTwoDimArray* MapA, UCellParamsTwoDimArray* MapB)
{
	if (!MapA) return MapB;
	if (!MapB) return MapA;	
	UCellParamsTwoDimArray* Map = UCellParamsTwoDimArray::New(MapA->GetLength().X, MapA->GetLength().Y);
	MapA->ForEach(FCellParamsTwoDimArrayIterator::CreateLambda(
		[Map, MapB](const FHexagonLocation Location, const FCellParameters& CellA)
	{
		const FCellParameters CellB = MapB->GetCell(Location);			
		const FCellParameters MixedCell = FCellParameters(Location,
				static_cast<ECellParametersType>(FMath::Min(static_cast<int>(CellA.ParametersType), static_cast<int>((CellB).ParametersType))),
				CellA.PlayerOwnerNumber,
				FMath::Min(CellA.ImproveLevel, CellB.ImproveLevel),
				FMath::Min(CellA.MinDistanceToClosestBuildingView, CellB.MinDistanceToClosestBuildingView),
				FMath::Min(CellA.MinDistanceToClosestAnchor, CellB.MinDistanceToClosestAnchor));
			
		Map->SetCell(Location, MixedCell);
	}));
	return Map;
}

void UCellParamsMap::Init(const ABuilding* Building, const FAnchorPoint& Anchor)
{
	TArray<FSingleAnchorData> Anchors = Anchor.GetAnchors(Building, 0);
	for (const FSingleAnchorData SingleAnchor : Anchors)
	{
		Cells = AddArrays(Cells ,GetCellsMapForSingleAnchor(Building, SingleAnchor));
	}
}

void UCellParamsMap::InitUnit(const AUnit* Unit, const FAnchorPoint& Anchor)
{
}

FCellParameters UCellParamsMap::GetCell(const FHexagonLocation Location) const
{
	if (Cells)
	{
		return Cells->GetCell(Location);
	}
	return FCellParameters();
}

UCellParamsTwoDimArray* UCellParamsMap::GetCellsMapForSingleAnchor(const ABuilding* Building, const FSingleAnchorData& SingleAnchor)
{
	UCellTwoDimArray* CellRefs = Building->GetFieldController()->GetCells();
	UCellParamsTwoDimArray* CellsMap = UCellParamsTwoDimArray::New(CellRefs->GetLength().X, CellRefs->GetLength().Y);
	
	CellRefs->ForEach(FCellTwoDimArrayIterator::CreateLambda(
		[Building, SingleAnchor, &CellsMap](FHexagonLocation Location, const ACell* CellRef)
	{
		ECellParametersType CellParametersType;
		if (Building->PlayerControllerRef->PlayerNumber != CellRef->GetPlayerOwnerNumber())
		{
			CellParametersType = ECellParametersType::NotBelongs;
		}
		else if (UFieldActorsFunctionLibrary::GetFieldActorsByLocation(Building, CellRef->GetLocation(), false, true).Num() > 1)
		{
			CellParametersType = ECellParametersType::IsOccupied;
		}
		else if (CheckTerrainRules(CellRef, Building->TerrainRules) == false)
		{
			CellParametersType = ECellParametersType::ForbiddenByTerrain;
		}
		else
		{
			CellParametersType = ECellParametersType::Free;
		}

		const int Distance = UHexagonFunctionLibrary::GetDistanceBetweenHexagons(CellRef->GetLocation(), SingleAnchor.Location);
		const int ImproveLevel = CalculateImprovementLevel(CellRef, Building, SingleAnchor, Distance);		
		if (CellParametersType == ECellParametersType::Free && ImproveLevel < SingleAnchor.MaximumDebuffLevelToBuild)
		{
			CellParametersType = ECellParametersType::ForbiddenByDebuffLevel;
		}

		int MinDistanceToClosestBuildingView = 99999;
		if (Building->BuildingViews.Num() > 0 || Building->PrefabViews.Num() > 0)
		{
			MinDistanceToClosestBuildingView = UHexagonFunctionLibrary::GetMinDistanceFromHexagons(Location,
				UHexagonFunctionLibrary::CombineHexagonArrays(
					UHexagonFunctionLibrary::ToHexagonsArray(TArray<AFieldActor*>(Building->BuildingViews)),
					UHexagonFunctionLibrary::ToHexagonsArray(TArray<AFieldActor*>(Building->PrefabViews))));
		}

		CellsMap->SetCell(Location,
			FCellParameters(Location, CellParametersType, CellRef->GetPlayerOwnerNumber(),
			ImproveLevel, MinDistanceToClosestBuildingView, Distance));	
	}));
	return CellsMap;
}

bool UCellParamsMap::CheckTerrainRules(const ACell* Cell, const FTerrainRules& TerrainRules)
{
	if (TerrainRules.DepthRulesType == ETerrainDepthRulesType::CannotBuildWithDepth && Cell->GetDepth() != 0)
	{
		return false;
	}
	if (TerrainRules.DepthRulesType == ETerrainDepthRulesType::NeedDepthToBuild && Cell->GetDepth() == 0)
	{
		return false;
	}	
	return TerrainRules.GetTerrainRule(Cell->GetTerrainType()).bCanBuild;
}

int UCellParamsMap::CalculateImprovementLevel(const ACell* Cell, const ABuilding* Building, const FSingleAnchorData SingleAnchor, const int Distance)
{
	const FSingleTerrainRule Rule = Building->TerrainRules.GetTerrainRule(Cell->GetTerrainType());
	const int BuildingRadius = SingleAnchor.Radius + Rule.BuildingRadiusAddition;
	int ImprovementLevel;	
	if (Distance > BuildingRadius)
	{
		const int Distance2 = Distance - BuildingRadius;
		ImprovementLevel = -1 - ((Distance2 - 1) / Building->GetFieldController()->GetDecreasingImproveLevelByRadius());		
	}
	else
	{
		ImprovementLevel = Distance <= 1 ? 1 : 0;
	}	
	return ImprovementLevel + Rule.ImproveLevelAddition;
}
