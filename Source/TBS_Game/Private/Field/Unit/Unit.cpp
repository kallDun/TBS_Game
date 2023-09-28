#include "Field/Unit/Unit.h"
#include <Net/UnrealNetwork.h>
#include "Field/Anchor/CellParamsMapGenerator.h"
#include "Field/Utils/UnitPlacementReturnState.h"
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

EUnitPlacementReturnState AUnit::CheckUnitPlacement()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EUnitPlacementReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EUnitPlacementReturnState::NotEnoughMoves;
	}
	if (AvailableUnitsCount <= 0)
	{
		return EUnitPlacementReturnState::ReachedLimit;
	}
	if (!CanPlace())
	{
		return EUnitPlacementReturnState::RequirementsNotMatch;
	}
	return EUnitPlacementReturnState::Succeeded;
}

EUnitPlacementReturnState AUnit::SetPreviewLocation(const FHexagonLocation HexagonLocation)
{
	if (!PrefabPreview)
	{
		return EUnitPlacementReturnState::Unknown;
	}
	PrefabPreview->SetActorHiddenInGame(true);

	const auto ReturnState = CheckUnitPlacement();
	if (ReturnState != EUnitPlacementReturnState::Succeeded) return ReturnState;
	
	if (!CanPlaceOnLocation(HexagonLocation))
	{
		return EUnitPlacementReturnState::LocationRequirementsNotMatch;
	}
	PrefabPreview->SetAndUpdateLocation(HexagonLocation);
	PrefabPreview->SetActorHiddenInGame(false);
	return EUnitPlacementReturnState::Succeeded;
}

EUnitPlacementReturnState AUnit::TryToPlace()
{
	const auto ReturnState = CheckUnitPlacement();
	if (ReturnState != EUnitPlacementReturnState::Succeeded) return ReturnState;
	
	PlacePrefabView();
	AvailableUnitsCount--;
	PlayerControllerRef->TryToUseMove();
	return EUnitPlacementReturnState::Succeeded;
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

// ------------------ Player move ------------------

void AUnit::AssembleMoveTick()
{
	for (AUnitView* View : UnitViews)
	{
		AssembleUnitViewMoveTick(View);
	}
}

void AUnit::AssembleUnitViewMoveTick(AUnitView* UnitView)
{
	UnitView->AssembleMoveTick();
}

void AUnit::StartMoveTick()
{
	if (UnitViews.Num() > 0)
	{
		StartUnitViewMoveTick(UnitViews[0]);
	}
	else EndMoveTick();
}

void AUnit::StartUnitViewMoveTick(AUnitView* UnitView)
{
	UnitView->StartMoveTick();
}

void AUnit::EndUnitViewMoveTick(AUnitView* UnitView)
{
	const int Index = UnitViews.Find(UnitView);
	if (Index < UnitViews.Num() - 1)
	{
		StartUnitViewMoveTick(UnitViews[Index + 1]);
	}
	else EndMoveTick();
}

void AUnit::EndMoveTick()
{
	PlayerControllerRef->EndUnitMove(this);
}
