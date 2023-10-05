#include "Field/Building/Building.h"
#include <Net/UnrealNetwork.h>
#include "Field/Anchor/CellParamsMapGenerator.h"
#include "Field/Building/BuildingView.h"
#include "Field/Building/UpgradeBuildingComponent.h"
#include "Field/Utils/BuildingPlacementReturnState.h"
#include "Field/Utils/BuildUpgradeReturnState.h"
#include "Player/GamePlayerController.h"
#include "Field/FieldActor.h"
#include "Field/Anchor/CellParameters.h"
#include "Field/Anchor/CellParametersType.h"
#include "Utils/TwoDimArray/CellParamsTwoDimArray.h"


void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Main properties
	DOREPLIFETIME( ABuilding, UpgradeBuildingComponents );
	DOREPLIFETIME( ABuilding, BuildingType );
	DOREPLIFETIME( ABuilding, BuildingViewClass );
	DOREPLIFETIME( ABuilding, bCanBuildWithOtherBuildings );
	DOREPLIFETIME( ABuilding, AffectingOnOtherBuildingImproveLevel );
	DOREPLIFETIME( ABuilding, InitMaxHitPoints );
	DOREPLIFETIME( ABuilding, InitMaxCellCount );
	DOREPLIFETIME( ABuilding, MovesToBuild );
	DOREPLIFETIME( ABuilding, MovesToAssemble );
	// Info properties
	DOREPLIFETIME( ABuilding, BuildActionInfo );
	DOREPLIFETIME( ABuilding, ImproveLevelFromLocationInfo );
	DOREPLIFETIME( ABuilding, PropertyChangedFromImproveLevelInfo );
	DOREPLIFETIME( ABuilding, RequirementsToExpendCells );
	// State properties
	DOREPLIFETIME( ABuilding, BuildingState );
	DOREPLIFETIME( ABuilding, CurrentLevel );
	DOREPLIFETIME( ABuilding, MovesToBuildLeft );
	DOREPLIFETIME( ABuilding, MovesToAssembleLeft );
	DOREPLIFETIME( ABuilding, BuildingViews );
	DOREPLIFETIME( ABuilding, PrefabViews );
	DOREPLIFETIME( ABuilding, PrefabPreview );
	DOREPLIFETIME( ABuilding, InitBuildingLocation );
}

void ABuilding::InitUpgradeBuildingComponents(TArray<UUpgradeBuildingComponent*> Components)
{
	for (auto Component : Components)
	{
		UpgradeBuildingComponents.Add(Component);
		Component->SetIsReplicated(true);
	}
}

// ----------------- Preview -----------------

void ABuilding::StartPreview()
{
	PrefabPreview = InitBuildingView(FHexagonLocation(), false);
	PrefabPreview->SetState(EBuildingViewState::Preview);
	PrefabPreview->SetActorHiddenInGame(true);
	CellParamsMap = UCellParamsMapGenerator::New(this);
	PlayerControllerRef->CellParamsMap = CellParamsMap;
}

EBuildingPlacementReturnState ABuilding::SetPreviewLocation(const FHexagonLocation HexagonLocation)
{
	PrefabPreview->SetActorHiddenInGame(true);
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildingPlacementReturnState::NotPlayerMove;
	}
	if (!CanBuild())
	{
		return EBuildingPlacementReturnState::RequirementsNotMet;
	}
	if (!CanBuildOnLocation(HexagonLocation))
	{
		return EBuildingPlacementReturnState::LocationRequirementsNotMet;
	}
	if (GetTotalUsedCells() >= GetMaxCellCount())
	{
		return EBuildingPlacementReturnState::NotEnoughAreaToBuild;
	}
	if (!CanExpendLocation(HexagonLocation))
	{
		return EBuildingPlacementReturnState::CannotExpandArea;
	}	
	if (PrefabPreview)
	{
		PrefabPreview->SetAndUpdateLocation(HexagonLocation);
		PrefabPreview->SetActorHiddenInGame(false);
	}
	return EBuildingPlacementReturnState::Succeeded;
}

void ABuilding::StopPreview()
{
	if (PrefabPreview)
	{
		PrefabPreview->Destroy();
		PrefabPreview = nullptr;
	}
	for (ABuildingView* const View : PrefabViews)
	{
		View->Destroy();
	}
	PrefabViews.Empty();
	CellParamsMap = nullptr;
	PlayerControllerRef->SetCellParamsMap(nullptr);
}


// ------------------ Cells Setup ------------------

EBuildingPlacementReturnState ABuilding::TryToExpendLocation(const FHexagonLocation HexagonLocation)
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildingPlacementReturnState::NotPlayerMove;
	}
	if (!CanBuild())
	{
		return EBuildingPlacementReturnState::RequirementsNotMet;
	}
	if (!CanBuildOnLocation(HexagonLocation))
	{
		return EBuildingPlacementReturnState::LocationRequirementsNotMet;
	}
	// TODO: check state so user can expend location ONLY if building is not built yet or after each upgrade
	if (GetTotalUsedCells() == 0)
	{
		InitBuildingLocation = HexagonLocation;
		PrefabViews.Add(InitBuildingView(HexagonLocation, true));
		CellParamsMap = UCellParamsMapGenerator::New(this);
		PlayerControllerRef->SetCellParamsMap(CellParamsMap);
		return EBuildingPlacementReturnState::Succeeded;
	}
	if (GetTotalUsedCells() < GetMaxCellCount())
	{
		if (!CanExpendLocation(HexagonLocation))
		{
			return EBuildingPlacementReturnState::CannotExpandArea;
		}
		PrefabViews.Add(InitBuildingView(HexagonLocation, false));
		CellParamsMap = UCellParamsMapGenerator::New(this);
		PlayerControllerRef->SetCellParamsMap(CellParamsMap);
		return EBuildingPlacementReturnState::Succeeded;
	}
	return EBuildingPlacementReturnState::NotEnoughAreaToBuild;
}

bool ABuilding::DeleteExpendedLocation(const FHexagonLocation HexagonLocation)
{
	ABuildingView* ViewToDelete = nullptr;
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
		const bool bIsMainView = ViewToDelete->GetLocation() == InitBuildingLocation;
		PrefabViews.Remove(ViewToDelete);
		ViewToDelete->Destroy();
		if (bIsMainView && PrefabViews.Num() > 0)
		{
			PrefabViews[0]->SetMainBuildingView(true);
		}
		CellParamsMap = UCellParamsMapGenerator::New(this);
		PlayerControllerRef->SetCellParamsMap(CellParamsMap);
	}
	return false;
}

ABuildingView* ABuilding::InitBuildingView(const FHexagonLocation HexagonLocation, const bool bIsMainView)
{
	ABuildingView* BuildingView = GetWorld()->SpawnActor<ABuildingView>(BuildingViewClass);
	BuildingView->Init(FieldController, this, HexagonLocation, bIsMainView);
	return BuildingView;
}

void ABuilding::BuildAllPrefabViews()
{
	for (ABuildingView* const View : PrefabViews)
	{
		BuildingViews.Add(View);
		View->SetState(EBuildingViewState::Building);
	}
	PrefabViews.Empty();
}

void ABuilding::ChangeBuildingViewsState(const EBuildingViewState NewState)
{
	for (ABuildingView* View : BuildingViews)
	{
		View->SetState(NewState);
	}
}


// ------------------ Build ------------------

bool ABuilding::CanBuild_Implementation() const
{
	return true;
}

EBuildUpgradeReturnState ABuilding::TryToBuild()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildUpgradeReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EBuildUpgradeReturnState::NotEnoughMoves;
	}
	if (BuildingState != EBuildingState::Initialized)
	{
		return EBuildUpgradeReturnState::IncorrectState;
	}
	if (!CanBuild())
	{
		return EBuildUpgradeReturnState::RequirementsNotMet;
	}
	if (GetTotalUsedCells() == 0)
	{
		return EBuildUpgradeReturnState::AreaNotDefined;
	}
	BuildingState = EBuildingState::Building;
	MovesToBuildLeft = MovesToBuild - 1;
	MovesToAssembleLeft = MovesToAssemble;
	const int ImproveLevel = GetImproveLevelFromLocation(InitBuildingLocation);
	ChangePropertiesAccordingToImproveLevel(ImproveLevel);
	BuildAllPrefabViews();
	if (MovesToBuildLeft == 0)
	{
		BuildingState = EBuildingState::Assembling;
		if (MovesToAssembleLeft == 0)
		{
			AssembleMoveTick();
		}
	}
	PlayerControllerRef->TryToUseMove();
	return EBuildUpgradeReturnState::Succeeded;
}

EBuildUpgradeReturnState ABuilding::TryToContinueBuilding()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildUpgradeReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EBuildUpgradeReturnState::NotEnoughMoves;
	}
	
	// TODO: check cell and return false state or change properties

	if (BuildingState == EBuildingState::Building)
	{
		MovesToBuildLeft--;
		if (MovesToBuildLeft <= 0)
		{
			BuildingState = EBuildingState::Assembling;
			if (MovesToAssembleLeft <= 0)
			{
				AssembleMoveTick();
			}
		}
		PlayerControllerRef->TryToUseMove();
		return EBuildUpgradeReturnState::Succeeded;
	}
	return EBuildUpgradeReturnState::IncorrectState;
}


// ------------------ Upgrade ------------------

EBuildUpgradeReturnState ABuilding::TryToUpgrade()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildUpgradeReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EBuildUpgradeReturnState::NotEnoughMoves;
	}
	
	UUpgradeBuildingComponent** UpgradeComponentRef = UpgradeBuildingComponents.FindByPredicate(
		[this](const UUpgradeBuildingComponent* UpgradeBuildingComponent)
		{
			return UpgradeBuildingComponent->LevelNumber == CurrentLevel + 1;
		});
	if (UpgradeComponentRef == nullptr)
	{
		return EBuildUpgradeReturnState::NoUpgradeAvailable;
	}
	UUpgradeBuildingComponent* UpgradeComponent = *UpgradeComponentRef;
	const EBuildUpgradeReturnState UpgradeState = UpgradeComponent->TryToUpgrade();
	if (UpgradeState == EBuildUpgradeReturnState::Succeeded)
	{
		PlayerControllerRef->TryToUseMove();
	}
	return UpgradeState;
}

EBuildUpgradeReturnState ABuilding::TryToContinueUpgrading()
{
	if (PlayerControllerRef->CheckIfPlayerMove() == false)
	{
		return EBuildUpgradeReturnState::NotPlayerMove;
	}
	if (PlayerControllerRef->CanUseMove() == false)
	{
		return EBuildUpgradeReturnState::NotEnoughMoves;
	}
	
	UUpgradeBuildingComponent** UpgradeComponentRef = UpgradeBuildingComponents.FindByPredicate(
		[this](const UUpgradeBuildingComponent* UpgradeBuildingComponent)
		{
			return UpgradeBuildingComponent->LevelNumber == CurrentLevel + 1
				&& UpgradeBuildingComponent->UpgradingState == EUpgradingState::Building;
		});
	if (UpgradeComponentRef == nullptr)
	{
		return EBuildUpgradeReturnState::NoUpgradeAvailable;
	}
	UUpgradeBuildingComponent* UpgradeComponent = *UpgradeComponentRef;
	const EBuildUpgradeReturnState UpgradeState = UpgradeComponent->TryToContinueUpgrading();
	if (UpgradeState == EBuildUpgradeReturnState::Succeeded)
	{
		PlayerControllerRef->TryToUseMove();
	}
	return UpgradeState;
}


// ------------------ Cells Map ------------------

int ABuilding::GetImproveLevelFromLocation(const FHexagonLocation HexagonLocation) const
{
	if (CellParamsMap)
	{
		return CellParamsMap->GetCell(HexagonLocation).ImproveLevel;
	}
	UE_LOG(LogTemp, Error, TEXT("ABuilding::GetImproveLevelFromLocation: CellParamsMap is nullptr"));
	return 0;
}

bool ABuilding::CanBuildOnLocation(const FHexagonLocation HexagonLocation) const
{
	if (CellParamsMap)
	{
		return CellParamsMap->GetCell(HexagonLocation).ParametersType == ECellParametersType::Free;
	}
	UE_LOG(LogTemp, Error, TEXT("ABuilding::CanBuildOnLocation: CellParamsMap is nullptr"));
	return false;
}

bool ABuilding::CanExpendLocation(const FHexagonLocation HexagonLocation) const
{
	if (GetTotalUsedCells() == 0) return true;
	if (CellParamsMap)
	{
		const FCellParameters Cell = CellParamsMap->GetCell(HexagonLocation);
		return Cell.ParametersType == ECellParametersType::Free
			&& Cell.MinDistanceToClosestBuildingView <= 1;
	}
	UE_LOG(LogTemp, Error, TEXT("ABuilding::CanExpendLocation: CellParamsMap is nullptr"));
	return false;
}


// ------------------ Player Move Tick ------------------

void ABuilding::StartPreMoveTick_Implementation()
{
	if (BuildingState != EBuildingState::Initialized)
	{
		for (UUpgradeBuildingComponent* Upgrade : UpgradeBuildingComponents)
		{
			Upgrade->PrePlayerMoveTick();
		}
		StartBuildingViewPreMove(BuildingViews[0]);
	}
	else
	{
		PlayerControllerRef->EndBuildingPreMove(this);
	}
}

void ABuilding::StartBuildingViewPreMove(ABuildingView* BuildingView)
{
	BuildingView->StartPreMoveTick();
}

void ABuilding::EndBuildingViewPreMove(ABuildingView* BuildingView)
{
	const int Index = BuildingViews.IndexOfByKey(BuildingView);
	if (Index < BuildingViews.Num() - 1)
	{
		StartBuildingViewPreMove(BuildingViews[Index + 1]);
	}
	else
	{
		PlayerControllerRef->EndBuildingPreMove(this);
	}
}

void ABuilding::StartPostMoveTick_Implementation()
{
	if (BuildingState != EBuildingState::Initialized)
	{
		for (UUpgradeBuildingComponent* Upgrade : UpgradeBuildingComponents)
		{
			Upgrade->PostPlayerMoveTick();
		}
		StartBuildingViewPostMove(BuildingViews[0]);
	}
	else
	{
		PlayerControllerRef->EndBuildingPostMove(this);
	}
}

void ABuilding::StartBuildingViewPostMove(ABuildingView* BuildingView)
{
	BuildingView->StartPostMoveTick();
}

void ABuilding::EndBuildingViewPostMove(ABuildingView* BuildingView)
{
	const int Index = BuildingViews.IndexOfByKey(BuildingView);
	if (Index < BuildingViews.Num() - 1)
	{
		StartBuildingViewPostMove(BuildingViews[Index + 1]);
	}
	else
	{
		PlayerControllerRef->EndBuildingPostMove(this);
	}
}

void ABuilding::AssembleMoveTick()
{
	if (BuildingState == EBuildingState::Assembling)
	{
		MovesToAssembleLeft--;
		if (MovesToAssembleLeft <= 0)
		{
			BuildingState = EBuildingState::Ready;
			ChangeBuildingViewsState(EBuildingViewState::Ready);
			OnBuildAction();
		}
	}
	else if (BuildingState == EBuildingState::Ready)
	{
		for (UUpgradeBuildingComponent* Upgrade : UpgradeBuildingComponents)
		{
			Upgrade->AssembleMoveTick();
		}
	}
}


// ------------------ Getters ------------------

TArray<FValueProperty> ABuilding::GetPropertiesByLevel(const bool UseCurrentLevel, const int CustomLevel) const
{
	TMap<FString, float> Map = {};
	for (FValueProperty Property : InitProperties)
	{
		Map.Add(Property.Name.ToString(), Property.Value);
	}
	
	for (UUpgradeBuildingComponent* Upgrade : GetUpgradeComponentsWithLevel(UseCurrentLevel ? CurrentLevel : CustomLevel, UseCurrentLevel))
	{
		for (FValueProperty Property : Upgrade->Properties)
		{
			if (Map.Contains(Property.Name.ToString()))
			{
				Map[Property.Name.ToString()] += Property.Value;
			}
			else 
			{
				Map.Add(Property.Name.ToString(), Property.Value);
			}
		}
	}

	TArray<FValueProperty> Result = {};
	for (auto& Elem : Map)
	{
		Result.Add(FValueProperty(FName(Elem.Key), Elem.Value));
	}
	return Result;
}

TArray<FValueProperty> ABuilding::GetCurrentProperties() const
{
	return GetPropertiesByLevel();
}

float ABuilding::GetMaxHitPoints(const bool UseCurrentLevel, const int CustomLevel) const
{
	float MaxHitPoints = InitMaxHitPoints;
	for (const UUpgradeBuildingComponent* Upgrade : GetUpgradeComponentsWithLevel(UseCurrentLevel ? CurrentLevel : CustomLevel, UseCurrentLevel))
	{
		MaxHitPoints += Upgrade->MaxHitPointsAddition;
	}
	return MaxHitPoints;
}

int ABuilding::GetMaxCellCount(const bool UseCurrentLevel, const int CustomLevel) const
{
	int MaxCellCount = InitMaxCellCount;
	for (const UUpgradeBuildingComponent* Upgrade :  GetUpgradeComponentsWithLevel(UseCurrentLevel ? CurrentLevel : CustomLevel, UseCurrentLevel))
	{
		MaxCellCount += Upgrade->MaxCellCountAddition;
	}
	return MaxCellCount;
}

TArray<UUpgradeBuildingComponent*> ABuilding::GetUpgradeComponentsWithLevel(const int Level, const bool OnlyReadyComponents) const
{
	TArray<UUpgradeBuildingComponent*> Result = {};
	for (UUpgradeBuildingComponent* Upgrade : UpgradeBuildingComponents)
	{
		if (Upgrade->LevelNumber <= Level)
		{
			if (!OnlyReadyComponents || Upgrade->UpgradingState == EUpgradingState::Ready)
			{
				Result.Add(Upgrade);
			}
		}
	}
	return Result;
}
