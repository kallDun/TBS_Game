#include "Field/Controller//FieldController.h"
#include <Net/UnrealNetwork.h>
#include "Field/Cell/Cell.h"
#include "Field/Cell/CellClassToTerrain.h"
#include "Field/Controller/FieldControllerState.h"
#include "Utils/TwoDimArray/CellTwoDimArray.h"
#include "Engine/ActorChannel.h"
#include "Player/GamePlayerController.h"


// --------------------------------- Init methods ----------------------------------

AFieldController::AFieldController()
{
	bReplicates = true;
}

void AFieldController::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GenerateField();
		OnPlayerTurnEnded.AddDynamic(this, &AFieldController::PlayerTurnEndedEventHandler);
	}
}


// -------------------------------- Network methods --------------------------------

void AFieldController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// General
	DOREPLIFETIME( AFieldController, HexagonSize );
	DOREPLIFETIME( AFieldController, FieldCenter );
	DOREPLIFETIME( AFieldController, DecreasingImproveLevelByRadius );
	DOREPLIFETIME( AFieldController, PlayersCount );
	DOREPLIFETIME( AFieldController, CellClasses );
	DOREPLIFETIME( AFieldController, MovesPerTurn );
	DOREPLIFETIME( AFieldController, BuildingClasses );
	DOREPLIFETIME( AFieldController, MainBuildingClassIndex );
	DOREPLIFETIME( AFieldController, UnitClasses );
	DOREPLIFETIME( AFieldController, HeroClasses );
	// State
	DOREPLIFETIME( AFieldController, Cells );
	DOREPLIFETIME( AFieldController, Players );
	DOREPLIFETIME( AFieldController, Turn );
	DOREPLIFETIME( AFieldController, State );
}

bool AFieldController::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bWroteSomething |= Channel->ReplicateSubobject(Cells, *Bunch, *RepFlags);
	return bWroteSomething;
}


// ---------------------------- Add & Check connected players ----------------------------

void AFieldController::AddPlayerToList(AGamePlayerController* Player)
{
	if (HasAuthority())
	{
		Players.Add(Player);
		if (Players.Num() == PlayersCount)
		{
			StartGame();
		}
	}
}

// -------------------------------- Main methods ----------------------------------

void AFieldController::StartGame_Implementation()
{
	State = EFieldControllerState::GameStarted;
	Turn = 0;
	Players[Turn]->StartTurn();
}

void AFieldController::NextTurn_Implementation(AGamePlayerController* Player)
{
	const int PlayerIndex = Players.IndexOfByKey(Player);
	Turn = (PlayerIndex + 1) % Players.Num();
	Players[Turn]->StartTurn();
}

void AFieldController::PlayerTurnEndedEventHandler(AGamePlayerController* Player)
{
	NextTurn(Player);
}

// ------------------------------ Auxiliary methods --------------------------------

TSubclassOf<ACell> AFieldController::GetCellClassByTerrainType(const ETerrainType TerrainType) const
{
	TArray<TArray<FCellClassToTerrain>::ElementType> ClassesWithNeedTerrain = CellClasses.FilterByPredicate(
		[TerrainType](const FCellClassToTerrain& CellClassToTerrain)
		{
			return CellClassToTerrain.TerrainType == TerrainType;
		});
	if (ClassesWithNeedTerrain.Num() == 0)
	{
		return nullptr;
	}
	const int RandomIndex = FMath::RandRange(0, ClassesWithNeedTerrain.Num() - 1);
	return ClassesWithNeedTerrain[RandomIndex].CellClass;	
}
