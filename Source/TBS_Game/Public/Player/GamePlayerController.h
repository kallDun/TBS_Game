#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PlayerTurnType.h"
#include "Field/GameActor.h"
#include "Field/HexagonLocation.h"
#include "GameFramework/Character.h"
#include "GamePlayerController.generated.h"

class AFieldActor;
class ATBS_GameModeBase;
class AUnit;
class AHero;
class ABuilding;
enum class EBuildUpgradeReturnState : uint8;
enum class EUnitPlacementReturnState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurnChanged, EPlayerTurnType, TurnType);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Player))
class TBS_GAME_API AGamePlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	AGamePlayerController();

// Components
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

// Default values
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Input")
	UInputMappingContext* PlayerInputMappingContext;

// State
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	EPlayerTurnType PlayerTurnType = EPlayerTurnType::Waiting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	FHexagonLocation CenterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int EconomicPoints = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int ReligiousFollowers = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int PlayerNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int MovesLeft = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated, ReplicatedUsing=OnRep_BuildingPrefabs)
	TArray<ABuilding*> BuildingPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	TArray<ABuilding*> Buildings = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated, ReplicatedUsing=OnRep_UnitPrefabs)
	TArray<AUnit*> Units = {};

	// Visual
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated,
		ReplicatedUsing=OnRep_CellParamsMap)
	class UCellParamsTwoDimArray* CellParamsMap = nullptr;

// Events
public:
	UPROPERTY(BlueprintAssignable)
	FPlayerEvent PlayerInitializeFinished;

	UPROPERTY(BlueprintAssignable)
	FPlayerTurnChanged PlayerTurnTypeChanged;

private:
	UFUNCTION(NetMulticast, Reliable)
	void PlayerInitializeFinishedBroadcast();

	UFUNCTION(NetMulticast, Reliable)
	void PlayerTurnTypeChangedBroadcast(const EPlayerTurnType NewPlayerTurnType);

	UFUNCTION(NetMulticast, Reliable)
	void PlayerTurnEndedBroadcast();

	// --------------------------------- not implemented in BPs ----------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AHero*> HeroPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	AHero* Hero;

// -------------------------------------------------------------------------------------------

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFieldController* FieldController;

// Getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }


// Base overrides
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Initialization
protected:
	UFUNCTION()
	void Init(int Index);

	UFUNCTION(Server, Reliable)
	void InitState(int PlayerNum, FHexagonLocation CenterHexLocation);

	UFUNCTION()
	void InitBuildingPrefabs();

	UFUNCTION()
	ABuilding* InitBuildingPrefab(TSubclassOf<ABuilding> BuildingClass);

	UFUNCTION()
	void InitUnitPrefabs();

	UFUNCTION()
	AUnit* InitUnitPrefab(TSubclassOf<AUnit> UnitClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitInputComponent();

// Methods
public:
	UFUNCTION()
	void StartTurn();

	UFUNCTION()
	bool CanUseMove() const;

	UFUNCTION()
	bool TryToUseMove();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckIfPlayerMove() const;

	UFUNCTION()
	void EndUnitMove(AUnit* Unit);

	UFUNCTION()
	void EndBuildingPreMove(ABuilding* Building);

	UFUNCTION()
	void EndBuildingPostMove(ABuilding* Building);

protected:
	UFUNCTION()
	void EndTurn();

	UFUNCTION()
	void SetPlayerTurnType(EPlayerTurnType NewPlayerTurnType);

	UFUNCTION()
	void StartUnitsMove();

	UFUNCTION()
	void StartUnitMove(AUnit* Unit);

	UFUNCTION()
	void DoUnitsAssembling();

	UFUNCTION()
	void DoBuildingsAssembling();

	UFUNCTION()
	void StartBuildingsPreMove();

	UFUNCTION()
	void StartBuildingPreMove(ABuilding* Building);

	UFUNCTION()
	void StartPlayerMove();

	UFUNCTION()
	void StartBuildingsPostMove();

	UFUNCTION()
	void StartBuildingPostMove(ABuilding* Building);

	// Actions
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState ConstructBuilding(ABuilding* Building);

	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState PlaceUnit(AUnit* Unit);

// Auxiliary
protected:
	UFUNCTION(BlueprintCallable)
	AActor* GetActorUnderCursor() const;

	UFUNCTION(BlueprintCallable)
	void SetActorLocationAndRotation(FHexagonLocation HexagonLocation, const FRotator& WorldRotation);

	UFUNCTION(Server, Reliable)
	void SetActorLocationAndRotationServer(FVector WorldLocation, FRotator WorldRotation);

// Replication
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_CellParamsMap();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_BuildingPrefabs();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_UnitPrefabs();
};
