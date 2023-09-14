#include "Field/Unit/Unit.h"

#include "Field/Anchor/CellParamsMapGenerator.h"
#include "Field/ReturnState/UnitPlacementReturnState.h"
#include "Field/ReturnState/UnitUpgradeReturnState.h"
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
	UE_LOG(LogTemp, Error, TEXT("ABuilding::GetImproveLevelFromLocation: CellParamsMap is nullptr"));
	return 0;
}



EUnitUpgradeReturnState AUnit::TryToBuild()
{
	return EUnitUpgradeReturnState::Succeeded;
}

bool AUnit::CanBuildOnLocation(FHexagonLocation HexagonLocation)
{
	int improveLevel = GetImproveLevelFromLocation(HexagonLocation);
	return improveLevel >= -1;
}

bool AUnit::CanSpawnInBuilding(ABuilding* Building)
{
	return true;
}

void AUnit::ReceiveDamage(float DamageAmount)
{
	if (UnitState != EUnitState::Destroyed)
	{
		CurrentHitPoints = FMath::Max(0.0f, CurrentHitPoints - DamageAmount);
		if (CurrentHitPoints <= 0)
		{
			UnitState = EUnitState::Destroyed;
			// Handle unit destruction logic here
		}
	}
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

bool AUnit::DeleteExpendedLocation(const FHexagonLocation HexagonLocation)
{
	AUnitView* ViewToDelete = nullptr;
	for (const auto View : PrefabViews)
	{
		if (View->GetLocation() == HexagonLocation)
		{
			ViewToDelete = View;
			break;
		}
	}
	if (ViewToDelete)
	{
		const bool bIsMainView = ViewToDelete->GetLocation() == InitUnitLocation;
		PrefabViews.Remove(ViewToDelete);
		ViewToDelete->Destroy();
		if (bIsMainView && PrefabViews.Num() > 0)
		{
		}
		//CellParamsMap = UCellParamsMapGenerator::FromUnit(this);
	}
	return false;
}


// ----------------- Preview -----------------

void AUnit::StartPreview()
{
	PrefabPreview = InitUnitView(FHexagonLocation());
	PrefabPreview->SetState(EUnitViewState::Preview);
	PrefabPreview->SetActorHiddenInGame(true);
	//CellParamsMap = UCellParamsMapGenerator::FromUnit(this);
}

void AUnit::StopPreview()
{
	if (PrefabPreview)
	{
		PrefabPreview->Destroy();
		PrefabPreview = nullptr;
	}
	for (AUnitView* const View : PrefabViews)
	{
		View->Destroy();
	}
	PrefabViews.Empty();
	CellParamsMap = nullptr;
}

AUnitView* AUnit::InitUnitView(const FHexagonLocation HexagonLocation)
{
	AUnitView* UnitView = GetWorld()->SpawnActor<AUnitView>(UnitViewClass);
	UnitView->Init(FieldController, this, HexagonLocation);
	return UnitView;
}