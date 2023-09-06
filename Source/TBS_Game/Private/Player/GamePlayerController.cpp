#include "Player/GamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Field/FieldActor.h"
#include "Field/Building/Building.h"
#include "Field/Controller/FieldController.h"
#include "Field/Event/TurnsOrderEventSystem.h"
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
			const int Index = Field->GetPlayersCount();
			Field->AddPlayerToList(this);
			if (Owner) Init(Index);
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
}

// -------------------------------- Initialization --------------------------------

void AGamePlayerController::Init(const int Index)
{
	InitInputComponent(); // Client
	SubscribeEventHandlersBP(FieldController->GetTurnsOrderEventSystem()); // Client
	const FHexagonLocation CenterHexLocation = FieldController->GetPlayerCenterLocation(Index);
	SetActorLocationAndRotation(CenterHexLocation, FRotator(0, 90, 0)); // Client
	InitState(Index, CenterHexLocation); // Server
}

void AGamePlayerController::InitState_Implementation(const int PlayerNum, const FHexagonLocation CenterHexLocation)
{
	PlayerNumber = PlayerNum;
	CenterLocation = CenterHexLocation;
	InitBuildingPrefabs();
	FieldController->GetTurnsOrderEventSystem()->PlayerInitializeFinished.Broadcast(this);
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

// -------------------------------- Player turn --------------------------------

void AGamePlayerController::StartTurn_Implementation()
{
	FieldController->GetTurnsOrderEventSystem()->PlayerTurnStarted.Broadcast(this);
	MovesLeft = FieldController->GetMovesPerTurn();
	// TODO: add realization for units move
	// TODO: add realization for check win condition

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(
		TEXT("StartTurn, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
	
	StartBuildingsAssembling();
}

void AGamePlayerController::EndTurn()
{
	SetPlayerTurnType(EPlayerTurnType::Waiting);	
	FieldController->GetTurnsOrderEventSystem()->PlayerTurnEnded.Broadcast(this);
}

bool AGamePlayerController::CanUseMove() const
{
	return MovesLeft > 0;
}

bool AGamePlayerController::TryToUseMove()
{
	if (MovesLeft > 0)
	{
		MovesLeft--;
		if (MovesLeft == 0)
		{
			StartBuildingsPostMove();
		}
		return true;
	}
	return false;
}

void AGamePlayerController::StartPlayersMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(
		TEXT("StartPlayersMove, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
	SetPlayerTurnType(EPlayerTurnType::PlayerMove);
}

void AGamePlayerController::StartBuildingsAssembling()
{
	SetPlayerTurnType(EPlayerTurnType::BuildingsAssembling);
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(
		TEXT("StartBuildingsAssembling, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
	
	for (ABuilding* Building : Buildings)
	{
		Building->AssembleMoveTick();		
	}
	StartBuildingsPreMove();
}

void AGamePlayerController::StartBuildingsPreMove()
{
	SetPlayerTurnType(EPlayerTurnType::BuildingsPreMove);
	BuildingMoveCalledCount = 0;
	FieldController->GetTurnsOrderEventSystem()->BuildingPreMoveEnded.AddDynamic(this, &AGamePlayerController::BuildingPreMoveEndedEventHandler);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(
		TEXT("StartBuildingsPreMove, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
	
	DoNextBuildingPreMove();
}

void AGamePlayerController::StartBuildingsPostMove()
{
	SetPlayerTurnType(EPlayerTurnType::BuildingsPostMove);
	BuildingMoveCalledCount = 0;
	FieldController->GetTurnsOrderEventSystem()->BuildingPostMoveEnded.AddDynamic(this, &AGamePlayerController::BuildingPostMoveEndedEventHandler);
	DoNextBuildingPostMove();
}

void AGamePlayerController::DoNextBuildingPreMove()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(
		TEXT("DoNextBuildingPreMove, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
	
	if (BuildingMoveCalledCount == Buildings.Num())
	{
		FieldController->GetTurnsOrderEventSystem()->BuildingPreMoveEnded.RemoveDynamic(this, &AGamePlayerController::BuildingPreMoveEndedEventHandler);
		StartPlayersMove();
	}
	else
	{
		Buildings[BuildingMoveCalledCount]->PrePlayerMoveTick();
	}
}

void AGamePlayerController::DoNextBuildingPostMove()
{
	if (BuildingMoveCalledCount == Buildings.Num())
	{
		FieldController->GetTurnsOrderEventSystem()->BuildingPostMoveEnded.RemoveDynamic(this, &AGamePlayerController::BuildingPostMoveEndedEventHandler);
		EndTurn();
	}
	else
	{
		Buildings[BuildingMoveCalledCount]->PostPlayerMoveTick();
	}
}

void AGamePlayerController::BuildingPreMoveEndedEventHandler(ABuilding* Building)
{
	if (Buildings.Contains(Building))
	{
		BuildingMoveCalledCount++;
		DoNextBuildingPreMove();
	}
}

void AGamePlayerController::BuildingPostMoveEndedEventHandler(ABuilding* Building)
{
	if (Buildings.Contains(Building))
	{
		BuildingMoveCalledCount++;
		DoNextBuildingPostMove();
	}
}

void AGamePlayerController::SetPlayerTurnType(const EPlayerTurnType NewPlayerTurnType)
{
	FieldController->GetTurnsOrderEventSystem()->PlayerTurnTypeChanged.Broadcast(this, NewPlayerTurnType);
	PlayerTurnType = NewPlayerTurnType;
}

bool AGamePlayerController::CheckIfPlayerMove() const
{
	return PlayerTurnType == EPlayerTurnType::PlayerMove;
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

