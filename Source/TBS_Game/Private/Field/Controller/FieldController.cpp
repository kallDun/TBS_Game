#include "Field/Controller//FieldController.h"

#include <Net/UnrealNetwork.h>

#include "Field/Cell/Cell.h"
#include "Field/Cell/CellClassToTerrain.h"
#include "Field/Controller/FieldControllerState.h"
#include "Field/Event/TurnsOrderEventSystem.h"


// --------------------------------- Init methods ----------------------------------

AFieldController::AFieldController()
{
	PrimaryActorTick.bCanEverTick = true;
	State = EFieldControllerState::WaitingForPlayers;
}


// -------------------------------- Network methods --------------------------------

void AFieldController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AFieldController, State );
	DOREPLIFETIME( AFieldController, Turn );
}


// ---------------------------- Check connected players ----------------------------

void AFieldController::AddPlayerToList(AGamePlayerController* Player)
{
	Players.Add(Player);

	const FString Message = FString::Printf(TEXT("Player %d is initialized. Total players = %d\nIsServer = %hs"),
		Players.IndexOfByKey(Player), Players.Num(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Message);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("FC Owner = %s"), *Owner.GetName()));
}

void AFieldController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetWorld()->GetNetMode() == NM_ListenServer)
	{
		CheckForPlayersInitialize();
	}
}

void AFieldController::CheckForPlayersInitialize()
{
	if (State == EFieldControllerState::WaitingForPlayers && IsAllPlayersInitialized())
	{
		InitializeEventSystem();
		GenerateField();
		InitializePlayers();
		StartGame();
	}
}

void AFieldController::InitializeEventSystem_Implementation()
{
	TurnsOrderEventSystem = NewObject<UTurnsOrderEventSystem>(this);
	TurnsOrderEventSystem->PlayerTurnEnded.AddDynamic(this, &AFieldController::PlayerTurnEndedEventHandler);
}


// -------------------------------- Main methods ----------------------------------

void AFieldController::GenerateField_Implementation()
{
	const FString Message = FString::Printf(TEXT("Generate Field!\nIsServer = %hs"),
			GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Message);
	State = EFieldControllerState::GeneratingField;
}

void AFieldController::InitializePlayers_Implementation()
{	
	for (int i = 0; i < Players.Num(); i++)
	{
		Players[i]->Init(i, GetPlayerCenterLocation(i));
	}
}

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

bool AFieldController::IsAllPlayersInitialized() const
{
	if (Players.Num() != GetPlayersCount()) return false;
	for (const AGamePlayerController* Player : Players)
	{
		if (Player->InitializationState == EPlayerInitializationState::NotInitialized) return false;
	}
	return true;
}
