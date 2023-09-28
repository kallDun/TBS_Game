#include "Field/Unit/Unit.h"
#include <Net/UnrealNetwork.h>
#include "Field/Anchor/CellParamsMapGenerator.h"
#include "Field/Utils/UnitPlacementReturnState.h"
#include "Field/Utils/UnitUpgradeReturnState.h"
#include "Player/GamePlayerController.h"
#include "Utils/TwoDimArray/CellParamsTwoDimArray.h"


void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// General properties
	DOREPLIFETIME( AUnit, UnitType );
	DOREPLIFETIME( AUnit, Level );
	DOREPLIFETIME( AUnit, UnitViewClass );
	DOREPLIFETIME( AUnit, InitMaxHitPoints );
	DOREPLIFETIME( AUnit, InitMaxDefence );
	DOREPLIFETIME( AUnit, InitMovement );
	DOREPLIFETIME( AUnit, MovesToAssemble );
	// State properties
	DOREPLIFETIME( AUnit, PrefabPreview );
	DOREPLIFETIME( AUnit, UnitViews );
}

// ------------------ Place ------------------

void AUnit::StartPreview()
{
	PrefabPreview = InitUnitView(FHexagonLocation());
	PrefabPreview->SetState(EUnitViewState::Preview);
	PrefabPreview->SetActorHiddenInGame(true);
	CellParamsMap = UCellParamsMapGenerator::New(this);
	PlayerControllerRef->CellParamsMap = CellParamsMap;
}

void AUnit::StopPreview()
{
	if (PrefabPreview)
	{
		PrefabPreview->Destroy();
		PrefabPreview = nullptr;
	}
	CellParamsMap = nullptr;
	PlayerControllerRef->CellParamsMap = nullptr;
}

EUnitPlacementReturnState AUnit::SetPreviewLocation(const FHexagonLocation HexagonLocation)
{
	PrefabPreview->SetActorHiddenInGame(true);
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EUnitPlacementReturnState::NotPlayerMove;
	}
	if (!CanPlace())
	{
		return EUnitPlacementReturnState::RequirementsNotMatch;
	}
	if (!CanPlaceOnLocation(HexagonLocation))
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

EUnitUpgradeReturnState AUnit::TryToPlace()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EUnitUpgradeReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EUnitUpgradeReturnState::NotEnoughMoves;
	}
	if (!CanPlace())
	{
		return EUnitUpgradeReturnState::RequirementsNotMatch;
	}
	PlacePrefabView();
	PlayerControllerRef->TryToUseMove();
	return EUnitUpgradeReturnState::Succeeded;
}

bool AUnit::CanPlaceOnLocation(const FHexagonLocation HexagonLocation) const
{
	if (CellParamsMap)
	{
		return CellParamsMap->GetCell(HexagonLocation).ParametersType == ECellParametersType::Free;
	}
	UE_LOG(LogTemp, Error, TEXT("ABuilding::CanBuildOnLocation: CellParamsMap is nullptr"));
	return false;
}

AUnitView* AUnit::InitUnitView(FHexagonLocation HexagonLocation)
{
	AUnitView* View = GetWorld()->SpawnActor<AUnitView>(UnitViewClass);
	View->Init(FieldController, this, HexagonLocation);
	return View;
}

void AUnit::PlacePrefabView()
{
	UnitViews.Add(PrefabPreview);
	PrefabPreview->StartAssembling();
	PrefabPreview = nullptr;
}
