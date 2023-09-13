#include "Player/GamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Field/FieldActor.h"
#include "Field/Building/Building.h"
#include "Field/Controller/FieldController.h"
#include "Field/ReturnState/BuildUpgradeReturnState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Utils/HexagonFunctionLibrary.h"


AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;
	ACharacter::SetReplicateMovement(true);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeRotation(FRotator(-80, 0, 0));
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		if (const auto Field = Cast<AFieldController>(GameState))
		{
			FieldController = Field;
			const int Index = Field->GetCurrentPlayersCount();
			if (Owner) Init(Index);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Player %d BeginPlay, Is Owner = %hhd"), Index, Owner != nullptr));
		}
	}
}


// ----------------------------------- Network -----------------------------------

void AGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AGamePlayerController, PlayerTurnType );
	DOREPLIFETIME( AGamePlayerController, CenterLocation );
	DOREPLIFETIME( AGamePlayerController, EconomicPoints );
	DOREPLIFETIME( AGamePlayerController, ReligiousFollowers );
	DOREPLIFETIME( AGamePlayerController, PlayerNumber );
	DOREPLIFETIME( AGamePlayerController, MovesLeft );
	DOREPLIFETIME( AGamePlayerController, BuildingPrefabs );
	DOREPLIFETIME( AGamePlayerController, Buildings );
	DOREPLIFETIME( AGamePlayerController, CellParamsMap );
}

// -------------------------------- Initialization --------------------------------

void AGamePlayerController::Init(const int Index)
{
	InitInputComponent(); // Client
	const FHexagonLocation CenterHexLocation = FieldController->GetPlayerCenterLocation(Index);
	SetActorLocationAndRotation(CenterHexLocation, FRotator(0, 90, 0)); // Client
	InitState(Index, CenterHexLocation); // Server
}

void AGamePlayerController::InitState_Implementation(const int PlayerNum, const FHexagonLocation CenterHexLocation)
{
	PlayerNumber = PlayerNum;
	CenterLocation = CenterHexLocation;
	InitBuildingPrefabs();
	FieldController->AddPlayerToList(this);
	PlayerInitializeFinishedBroadcast();
}

void AGamePlayerController::SetActorLocationAndRotation(const FHexagonLocation HexagonLocation, const FRotator& WorldRotation)
{
	FVector WorldLocation = UHexagonFunctionLibrary::GetWorldLocationFromHexagon(HexagonLocation,
			FieldController->GetHexagonSize(), FieldController->GetFieldCenter());
	WorldLocation.Z = 5000;
	SetActorLocationAndRotationServer(WorldLocation, WorldRotation);
	GetPlayerController()->SetReplicateMovement(true);
	SetActorRotation(WorldRotation);
}

void AGamePlayerController::SetActorLocationAndRotationServer_Implementation(const FVector WorldLocation, const FRotator WorldRotation)
{
	SetActorLocation(WorldLocation);
	SetActorRotation(WorldRotation);
}

void AGamePlayerController::InitBuildingPrefabs()
{
	TArray<TSubclassOf<ABuilding>> BuildingPrefabsClasses = FieldController->GetBuildingClasses();
	for (const TSubclassOf<ABuilding> BuildingClass : BuildingPrefabsClasses)
	{
		BuildingPrefabs.Add(InitBuildingPrefab(BuildingClass));
	}
}

ABuilding* AGamePlayerController::InitBuildingPrefab(const TSubclassOf<ABuilding> BuildingClass)
{
	ABuilding* BuildingPrefab = GetWorld()->SpawnActor<ABuilding>(BuildingClass);
	BuildingPrefab->Init(FieldController, this);
	BuildingPrefab->Owner = this;
	return BuildingPrefab;
}

// ----------------------------------- Events ----------------------------------

void AGamePlayerController::PlayerInitializeFinishedBroadcast_Implementation()
{
	PlayerInitializeFinished.Broadcast();
}

void AGamePlayerController::PlayerTurnTypeChangedBroadcast_Implementation(const EPlayerTurnType NewPlayerTurnType)
{
	PlayerTurnTypeChanged.Broadcast(NewPlayerTurnType);
}

void AGamePlayerController::PlayerTurnEndedBroadcast_Implementation()
{
	FieldController->OnPlayerTurnEnded.Broadcast(this);
}


// -------------------------------- Player turn --------------------------------

bool AGamePlayerController::CheckIfPlayerMove() const { return PlayerTurnType == EPlayerTurnType::PlayerMove; }

bool AGamePlayerController::CanUseMove() const { return MovesLeft > 0; }

void AGamePlayerController::StartTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d TURN STARTED"), PlayerNumber));

	MovesLeft = FieldController->GetMovesPerTurn();
	//StartUnitsMove();
	StartBuildingsAssembling();
}

void AGamePlayerController::EndTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d TURN ENDED"), PlayerNumber));
	
	SetPlayerTurnType(EPlayerTurnType::Waiting);
	PlayerTurnEndedBroadcast();
}
 
void AGamePlayerController::SetPlayerTurnType(const EPlayerTurnType NewPlayerTurnType)
{
	PlayerTurnTypeChangedBroadcast(NewPlayerTurnType);
	PlayerTurnType = NewPlayerTurnType;
}

bool AGamePlayerController::TryToUseMove()
{
	if (PlayerTurnType == EPlayerTurnType::PlayerMove && MovesLeft > 0)
	{
		MovesLeft--;
		return true;
	}
	return false;
}

void AGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		if (PlayerTurnType == EPlayerTurnType::PlayerMove && MovesLeft == 0)
		{
			StartBuildingsPostMove();
		}
	}
}

// -------------------------------- Move cycle ---------------------------------

void AGamePlayerController::StartUnitsMove()
{
	// TODO: implement with Units
}

void AGamePlayerController::StartUnitMove(AUnit* Unit)
{
	// TODO: implement with Units
}

void AGamePlayerController::EndUnitMove(AUnit* Unit)
{
	// TODO: implement with Units
}

void AGamePlayerController::StartBuildingsAssembling()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d start buildings ASSEMBLING"), PlayerNumber));
	
	SetPlayerTurnType(EPlayerTurnType::BuildingsAssembling);
	for (ABuilding* Building : Buildings)
	{
		Building->AssembleMoveTick();		
	}
	StartBuildingsPreMove();
}

void AGamePlayerController::StartBuildingsPreMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d start buildings PRE MOVE"), PlayerNumber));
	
	SetPlayerTurnType(EPlayerTurnType::BuildingsPreMove);
	if (Buildings.Num() > 0)
	{
		StartBuildingPreMove(Buildings[0]);
	}
	else EndBuildingPreMove(nullptr);
}

void AGamePlayerController::StartBuildingPreMove(ABuilding* Building)
{
	Building->StartPreMoveTick();
}

void AGamePlayerController::EndBuildingPreMove(ABuilding* Building)
{
	if (Building)
	{
		const int Index = Buildings.IndexOfByKey(Building);
		if (Index < Buildings.Num() - 1)
		{
			StartBuildingPreMove(Buildings[Index + 1]);
		}
		else StartPlayerMove();
	}
	else StartPlayerMove();
}

void AGamePlayerController::StartPlayerMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d MOVE STARTED"), PlayerNumber));
	SetPlayerTurnType(EPlayerTurnType::PlayerMove);
}

void AGamePlayerController::StartBuildingsPostMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("Player %d start buildings POST MOVE"), PlayerNumber));
	
	SetPlayerTurnType(EPlayerTurnType::BuildingsPostMove);
	if (Buildings.Num() > 0)
	{
		StartBuildingPostMove(Buildings[0]);
	}
	else StartBuildingPostMove(nullptr);
}

void AGamePlayerController::StartBuildingPostMove(ABuilding* Building)
{
	Building->StartPostMoveTick();
}

void AGamePlayerController::EndBuildingPostMove(ABuilding* Building)
{
	if (Building)
	{
		const int Index = Buildings.IndexOfByKey(Building);
		if (Index < Buildings.Num() - 1)
		{
			StartBuildingPostMove(Buildings[Index + 1]);
		}
		else EndTurn();
	}
	else EndTurn();
}

// -------------------------------- Actions --------------------------------

EBuildUpgradeReturnState AGamePlayerController::ConstructBuilding(ABuilding* Building)
{
	if (BuildingPrefabs.Contains(Building))
	{
		const EBuildUpgradeReturnState ReturnState = Building->TryToBuild();
		if (ReturnState == EBuildUpgradeReturnState::Succeeded)
		{
			BuildingPrefabs.Remove(Building);
			Buildings.Add(Building);
			BuildingPrefabs.Add(InitBuildingPrefab(Building->GetClass()));
		}
		return ReturnState;
	}
	return EBuildUpgradeReturnState::Unknown;
}


// -------------------------------- Input --------------------------------

void AGamePlayerController::InitInputComponent_Implementation()
{
	if (GetPlayerController())
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}
	}
	
	// Mouse input
	GetPlayerController()->SetShowMouseCursor(true);
}

AActor* AGamePlayerController::GetActorUnderCursor() const
{
	FVector WorldLocation;
	FVector WorldDirection;
	if (bool bIsMouseCursorOnViewport = GetPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		const float RayLength = 100000;
		FHitResult HitResult;
		if (const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * RayLength, ECC_Visibility))
		{
			return HitResult.GetActor();
		}
	}
	return nullptr;
}
