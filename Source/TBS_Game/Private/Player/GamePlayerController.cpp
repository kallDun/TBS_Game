#include "Player/GamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Field/FieldActor.h"
#include "Field/Building/Building.h"
#include "Field/Unit/Unit.h"
#include "Field/Hero/Hero.h"
#include "Field/Controller/FieldController.h"
#include "Field/Event/TurnsOrderEventSystem.h"
#include "Field/ReturnState/BuildUpgradeReturnState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Utils/HexagonFunctionLibrary.h"


AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;
	SetReplicatingMovement(true);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeRotation(FRotator(-80, 0, 0));
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	const FString Message = FString::Printf(TEXT("AGamePlayerController BeginPlay, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Message);
	
	if (AActor* GameState = UGameplayStatics::GetGameState(GetWorld()))
	{
		FieldController = Cast<AFieldController>(GameState);
		if (FieldController)
		{
			FieldController->AddPlayerToList(this);
		}
	}
	if (Owner)
	{
		SetInitializationStateLoaded();
	}
}

void AGamePlayerController::SetInitializationStateLoaded_Implementation()
{
	InitializationState = EPlayerInitializationState::Loaded;
	const FString Message = FString::Printf(TEXT("PlayerInitializationState::Loaded, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Message);
}


// ----------------------------------- Network -----------------------------------

void AGamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AGamePlayerController, InitializationState );
	DOREPLIFETIME( AGamePlayerController, EconomicPoints );
	DOREPLIFETIME( AGamePlayerController, ReligiousFollowers );
	DOREPLIFETIME( AGamePlayerController, MovesLeft );
	DOREPLIFETIME( AGamePlayerController, PlayerNumber );
	DOREPLIFETIME( AGamePlayerController, CenterLocation );
}

// -------------------------------- Initialization --------------------------------

void AGamePlayerController::Init_Implementation(const int PlayerNum, const FHexagonLocation CenterHexLocation)
{
	InitPlayerNumberAndCenterLocation(PlayerNum, CenterHexLocation);
	SetActorLocationAndRotation(CenterHexLocation, FRotator(0, 90, 0));
	InitInputComponent();
	InitBuildingPrefabs();
	InitEventHandlersBP(FieldController->GetTurnsOrderEventSystem());
	PlayerInitializedEvent.Broadcast();
	InitializationState = EPlayerInitializationState::Initialized;

	const FString Message2 = FString::Printf(TEXT("Player Init, Owner = %s\nIsServer = %hs"),
		*Owner.GetName(), GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Message2);
}

void AGamePlayerController::InitPlayerNumberAndCenterLocation_Implementation(const int PlayerNum, const FHexagonLocation HexagonLocation)
{
	PlayerNumber = PlayerNum;
	CenterLocation = HexagonLocation;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("PlayerNum = %d\nIsServer = %hs"),
		PlayerNum, GetWorld()->GetNetMode() == NM_ListenServer ? "Yes" : "No"));
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

void AGamePlayerController::InitBuildingPrefabs_Implementation()
{
	TArray<TSubclassOf<ABuilding>> BuildingPrefabsClasses = FieldController->GetBuildingClasses();
	for (const TSubclassOf<ABuilding> BuildingClass : BuildingPrefabsClasses)
	{
		BuildingPrefabs.Add(InitBuildingPrefab(BuildingClass));
	}
}

void AGamePlayerController::InitUnitPrefabs_Implementation()
{
	TArray<TSubclassOf<AUnit>> UnitPrefabsClasses = FieldController->GetUnitClasses();
	for (const TSubclassOf<AUnit> UnitClass : UnitPrefabsClasses)
	{
		UnitPrefabs.Add(InitUnitPrefab(UnitClass));
	}
}

void AGamePlayerController::InitHeroPrefabs_Implementation()
{
	TArray<TSubclassOf<AHero>> HeroPrefabsClasses = FieldController->GetHeroClasses();
	for (const TSubclassOf<AHero> HeroClass : HeroPrefabsClasses)
	{
		HeroPrefabs.Add(InitHeroPrefab(HeroClass));
	}
}

ABuilding* AGamePlayerController::InitBuildingPrefab(const TSubclassOf<ABuilding> BuildingClass)
{
	ABuilding* BuildingPrefab = GetWorld()->SpawnActor<ABuilding>(BuildingClass);
	BuildingPrefab->Init(FieldController, this);
	return BuildingPrefab;
}

AUnit* AGamePlayerController::InitUnitPrefab(const TSubclassOf<AUnit> UnitClass)
{
	AUnit* UnitPrefab = GetWorld()->SpawnActor<AUnit>(UnitClass);
	UnitPrefab->Init(FieldController, this);
	return UnitPrefab;
}

AHero* AGamePlayerController::InitHeroPrefab(const TSubclassOf<AHero> HeroClass)
{
	AHero* HeroPrefab = GetWorld()->SpawnActor<AHero>(HeroClass);
	HeroPrefab->Init(FieldController, this);
	return HeroPrefab;
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

EUnitUpgradeReturnState AGamePlayerController::ConstructUnit(AUnit* Unit)
{
	if (UnitPrefabs.Contains(Unit))
	{
		const EUnitUpgradeReturnState ReturnState = Unit->TryToBuild();
		if (ReturnState == EUnitUpgradeReturnState::Succeeded)
		{
			UnitPrefabs.Remove(Unit);
			Units.Add(Unit);
			UnitPrefabs.Add(InitUnitPrefab(Unit->GetClass()));
		}
		return ReturnState;
	}
	return EUnitUpgradeReturnState::Unknown;
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

