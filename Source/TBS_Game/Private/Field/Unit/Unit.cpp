#include "Field/Unit/Unit.h"
#include "Field/Utils/UnitPlacementReturnState.h"
#include "Field/Utils/UnitUpgradeReturnState.h"
#include "Player/GamePlayerController.h"

void AUnit::Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner)
{
	PlayerControllerRef = PlayerControllerOwner;
	AGameActor::Init(Field);
}

// ------------------ Cells Map ------------------

int AUnit::GetImproveLevelFromLocation(const FHexagonLocation HexagonLocation) const
{
	/*if (CellParamsMap)
	{
		return CellParamsMap->GetCell(HexagonLocation).ImproveLevel;
	}*/
	return 0;
}




bool AUnit::CanBuildOnLocation(FHexagonLocation HexagonLocation)
{
	int improveLevel = GetImproveLevelFromLocation(HexagonLocation);
	return improveLevel >= -1;
}



EUnitPlacementReturnState AUnit::SetPreviewLocation(const FHexagonLocation HexagonLocation)
{
	PrefabPreview->SetActorHiddenInGame(true);
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EUnitPlacementReturnState::NotPlayerMove;
	}
	if (!CanBuildOnLocation(HexagonLocation))
	{
		return EUnitPlacementReturnState::LocationRequirementsNotMatch;
	}
	if (PrefabPreview)
	{
		PrefabPreview->SetAndUpdateLocation(HexagonLocation);
		PrefabPreview->SetActorHiddenInGame(false);
	}
	return EUnitPlacementReturnState::Succeeded;
}

void AUnit::StopPreview()
{
}

EUnitUpgradeReturnState AUnit::TryToPlace()
{
	return EUnitUpgradeReturnState::RequirementsNotMatch;
}


// ----------------- Preview -----------------
