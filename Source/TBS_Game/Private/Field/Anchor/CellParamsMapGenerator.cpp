#include "Field/Anchor/CellParamsMapGenerator.h"
#include "Field/Building/Building.h"
#include "Field/Unit/Unit.h"
#include "Field/Cell/Cell.h"
#include "Field/Building/BuildingView.h"
#include "Field/Controller/FieldController.h"
#include "Player/GamePlayerController.h"
#include "Utils/FieldActorsFunctionLibrary.h"
#include "Field/FieldActor.h"
#include "Utils/HexagonFunctionLibrary.h"
#include "Utils/TwoDimArray/CellParamsTwoDimArray.h"
#include "Utils/TwoDimArray/CellTwoDimArray.h"


UCellParamsTwoDimArray* UCellParamsMapGenerator::New(const AFieldActorsHandler* Actor)
{
	UCellParamsTwoDimArray* Cells = nullptr;
	for (const FAnchorPoint Anchor : Actor->AnchorPoints)
	{
		Cells = AddArrays(Cells, InitFromAnchor(Actor, Anchor));
	}
	return Cells;
}

UCellParamsTwoDimArray* UCellParamsMapGenerator::AddArrays(UCellParamsTwoDimArray* MapA, UCellParamsTwoDimArray* MapB)
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

UCellParamsTwoDimArray* UCellParamsMapGenerator::InitFromAnchor(const AFieldActorsHandler* Actor, const FAnchorPoint& Anchor)
{
	UCellParamsTwoDimArray* Cells = nullptr;

	if (const ABuilding* Building = Cast<ABuilding>(Actor))
	{
		TArray<FSingleAnchorData> Anchors = Anchor.GetAnchors(Actor, 0);
		for (const FSingleAnchorData SingleAnchor : Anchors)
		{
			Cells = AddArrays(Cells ,GetBuildingRelatedCellsMapForSingleAnchor(Building, SingleAnchor));
		}
		return Cells;
	}

	if (const AUnit* Unit = Cast<AUnit>(Actor))
	{
		TArray<FSingleAnchorData> Anchors = Anchor.GetAnchors(Actor, 0);
		for (const FSingleAnchorData SingleAnchor : Anchors)
		{
			Cells = AddArrays(Cells ,GetUnitRelatedCellsMapForSingleAnchor(Unit, SingleAnchor));
		}
		return Cells;
	}

	return Cells;
}

UCellParamsTwoDimArray* UCellParamsMapGenerator::GetBuildingRelatedCellsMapForSingleAnchor(const ABuilding* Building, const FSingleAnchorData& SingleAnchor)
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

UCellParamsTwoDimArray* UCellParamsMapGenerator::GetUnitRelatedCellsMapForSingleAnchor(const AUnit* Unit,
	const FSingleAnchorData& SingleAnchor)
{
	UCellTwoDimArray* CellRefs = Unit->GetFieldController()->GetCells();
	UCellParamsTwoDimArray* CellsMap = UCellParamsTwoDimArray::New(CellRefs->GetLength().X, CellRefs->GetLength().Y);
	
	CellRefs->ForEach(FCellTwoDimArrayIterator::CreateLambda(
		[Unit, SingleAnchor, &CellsMap](FHexagonLocation Location, const ACell* CellRef)
	{
		ECellParametersType CellParametersType;
		if (Unit->PlayerControllerRef->PlayerNumber != CellRef->GetPlayerOwnerNumber())
		{
			CellParametersType = ECellParametersType::NotBelongs;
		}
		else if (UFieldActorsFunctionLibrary::GetFieldActorsByLocation(Unit, CellRef->GetLocation(), false, true).Num() > 1)
		{
			CellParametersType = ECellParametersType::IsOccupied;
		}
		else if (CheckTerrainRules(CellRef, Unit->TerrainRules) == false)
		{
			CellParametersType = ECellParametersType::ForbiddenByTerrain;
		}
		else
		{
			CellParametersType = ECellParametersType::Free;
		}

		const int Distance = UHexagonFunctionLibrary::GetDistanceBetweenHexagons(CellRef->GetLocation(), SingleAnchor.Location);
		const int ImproveLevel = CalculateImprovementLevel(CellRef, Unit, SingleAnchor, Distance);
		if (CellParametersType == ECellParametersType::Free && ImproveLevel < SingleAnchor.MaximumDebuffLevelToBuild)
		{
			CellParametersType = ECellParametersType::ForbiddenByDebuffLevel;
		}

		int MinDistanceToClosestBuildingView = 99999;
		if (Unit->UnitViews.Num() > 0)
		{
			MinDistanceToClosestBuildingView = UHexagonFunctionLibrary::GetMinDistanceFromHexagons(Location,
				UHexagonFunctionLibrary::ToHexagonsArray(TArray<AFieldActor*>(Unit->UnitViews)));
		}

		CellsMap->SetCell(Location,
			FCellParameters(Location, CellParametersType, CellRef->GetPlayerOwnerNumber(),
			ImproveLevel, MinDistanceToClosestBuildingView, Distance));	
	}));
	return CellsMap;
}

bool UCellParamsMapGenerator::CheckTerrainRules(const ACell* Cell, const FTerrainRules& TerrainRules)
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

int UCellParamsMapGenerator::CalculateImprovementLevel(const ACell* Cell, const AFieldActorsHandler* Actor, const FSingleAnchorData SingleAnchor, const int Distance)
{
	const FSingleTerrainRule Rule = Actor->TerrainRules.GetTerrainRule(Cell->GetTerrainType());
	const int BuildingRadius = SingleAnchor.Radius + Rule.BuildingRadiusAddition;
	int ImprovementLevel;
	if (Distance > BuildingRadius)
	{
		const int Distance2 = Distance - BuildingRadius;
		ImprovementLevel = -1 - ((Distance2 - 1) / Actor->GetFieldController()->GetDecreasingImproveLevelByRadius());		
	} 
	else
	{
		ImprovementLevel = Distance <= 1 ? 1 : 0;
	}	
	return ImprovementLevel + Rule.ImproveLevelAddition;
}
