#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "FieldControllerState.h"
#include "Field/HexagonLocation.h"
#include "GameFramework/GameState.h"
#include "FieldController.generated.h"

class ACell;
class UCellTwoDimArray;
class AUnit;
class AHero;
class ABuilding;
class AGamePlayerController;
class ATBS_GameStateBase;
class ATBS_GameModeBase;
class UTurnsOrderEventSystem;
enum class ETerrainType : uint8;
struct FCellClassToTerrain;

UCLASS(Abstract, BlueprintType, ClassGroup = (Field))
class TBS_GAME_API AFieldController : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFieldController();

// Defaults properties
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Getter = GetHexagonSize, meta = (AllowPrivateAccess = "true", ClampMin = 0), Category = "Field Settings", Replicated)
	float HexagonSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Getter = GetFieldCenter, meta = (AllowPrivateAccess = "true"), Category = "Field Settings", Replicated)
	FVector FieldCenter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Getter = GetDecreasingImproveLevelByRadius, meta = (AllowPrivateAccess = "true", ClampMin = 1), Category = "Field Settings", Replicated)
	int DecreasingImproveLevelByRadius = 5;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Getter = GetPlayersCount, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Field Settings", Replicated)
	int PlayersCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Field Settings", Replicated)
	TArray<FCellClassToTerrain> CellClasses;

	UPROPERTY(EditDefaultsOnly, Getter = GetMovesPerTurn, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 5), Category = "Player Settings", Replicated)
	int MovesPerTurn = 1;
	
	UPROPERTY(EditDefaultsOnly, Getter = GetBuildingClasses, meta = (AllowPrivateAccess = "true"), Category = "Player Settings", Replicated)
	TArray<TSubclassOf<ABuilding>> BuildingClasses;
	
	UPROPERTY(EditDefaultsOnly, Getter = GetMainBuildingClassIndex, meta = (AllowPrivateAccess = "true", ClampMin = 0), Category = "Player Settings", Replicated)
	int MainBuildingClassIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Getter = GetUnitClasses, meta = (AllowPrivateAccess = "true"), Category = "Player Settings", Replicated)
	TArray<TSubclassOf<AUnit>> UnitClasses;

	UPROPERTY(EditDefaultsOnly, Getter = GetHeroClasses, meta = (AllowPrivateAccess = "true"), Category = "Player Settings", Replicated)
	TArray<TSubclassOf<AHero>> HeroClasses;


// Properties
protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	UTurnsOrderEventSystem* TurnsOrderEventSystem;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	UCellTwoDimArray* Cells;	

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	TArray<AGamePlayerController*> Players = {};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	int Turn = 0;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated) // ---- DEPRECATED ----
	EFieldControllerState State = EFieldControllerState::NotInitialized;

// Getters
public:
	UFUNCTION(BlueprintGetter)
	UTurnsOrderEventSystem* GetTurnsOrderEventSystem() const { return TurnsOrderEventSystem; }
	
	UFUNCTION(BlueprintGetter)
	float GetHexagonSize() const { return HexagonSize; }

	UFUNCTION(BlueprintGetter)
	FVector GetFieldCenter() const { return FieldCenter; }

	UFUNCTION(BlueprintGetter)
	int GetDecreasingImproveLevelByRadius() const { return DecreasingImproveLevelByRadius; }
	
	UFUNCTION(BlueprintGetter)
	UCellTwoDimArray* GetCells() const { return Cells; }
	
	UFUNCTION(BlueprintGetter)
	int GetMovesPerTurn() const { return MovesPerTurn; }

	UFUNCTION(BlueprintGetter)
	int GetMainBuildingClassIndex() const { return MainBuildingClassIndex; }
	
	UFUNCTION(BlueprintGetter)
	TArray<TSubclassOf<ABuilding>> GetBuildingClasses() const { return BuildingClasses; }

	UFUNCTION(BlueprintGetter)
	TArray<TSubclassOf<AUnit>> GetUnitClasses() const { return UnitClasses; }

	UFUNCTION(BlueprintGetter)
	TArray<TSubclassOf<AHero>> GetHeroClasses() const { return HeroClasses; }
	
	UFUNCTION(BlueprintGetter)
	int GetPlayersCount() const { return PlayersCount; }

	
// Methods
public:
	UFUNCTION()
	void AddPlayerToList(AGamePlayerController* Player);

	UFUNCTION(BlueprintCallable)
	virtual FHexagonLocation GetPlayerCenterLocation(const int PlayerIndex) const { return FHexagonLocation(); }

protected:
	UFUNCTION(Server, Reliable)
	virtual void StartGame();

	UFUNCTION()
	virtual void GenerateField() {}

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	UFUNCTION(Server, Reliable)
	void NextTurn(AGamePlayerController* Player);
	
	UFUNCTION()
	void InitializeEventSystem();

// Auxiliary methods
protected:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<ACell> GetCellClassByTerrainType(const ETerrainType TerrainType) const;
	
// Event handlers
private:
	UFUNCTION()
	void PlayerTurnEndedEventHandler(AGamePlayerController* Player);
};
