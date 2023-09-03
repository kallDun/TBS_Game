#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "Field/FieldActor.h"
#include "Field/Anchor/TerrainRules.h"
#include "UnitState.h"
#include "UnitView.h"
#include "Unit.generated.h"

class ACell;
class ABuilding;
class AUnitView;
class UCellParamsMap;
class AGamePlayerController;
enum class EUnitPillarType : uint8;
enum class EUnitAttackType : uint8;
enum class EUnitPlacementReturnState : uint8;
enum class EUnitUpgradeReturnState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Unit))
class TBS_GAME_API AUnit : public AFieldActor
{
	GENERATED_BODY()

public:
	AUnit() {}
	void Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	EUnitPillarType UnitType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	USceneComponent* UnitMeshRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	TSubclassOf<AUnitView> UnitViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	FTerrainRules TerrainRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main")
	float InitMaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Main")
	float InitMaxDefence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.5", ClampMax = "5"), Category = "Unit Main")
	float NumSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "3"), Category = "Unit Main")
	int Level = 1;

	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	UTexture2D* UnitIconSmall;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	UTexture2D* UnitIconMedium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	FName UnitDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	FName UnitActionInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	FName MainRequirementsToSpawnInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info")
	FName ImproveLevelFromLocationInfo;

	// current state
	UPROPERTY(BlueprintGetter=GetPlayerControllerRef, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	AGamePlayerController* PlayerControllerRef = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	FHexagonLocation InitHeroLocation;

	UPROPERTY(BlueprintGetter=GetUnitState, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	EUnitState UnitState = EUnitState::Initialized;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	AUnitView* PrefabPreview;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	TArray<AUnitView*> PrefabViews = {};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	UCellParamsMap* CellParamsMap = nullptr;

	
// Getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGamePlayerController* GetPlayerControllerRef() const { return PlayerControllerRef; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHitPoints() const { return InitMaxHitPoints; };
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EUnitState GetUnitState() const { return UnitState; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxDefence() const { return InitMaxDefence; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetStepsNumber() const { return NumSteps; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentHitPoints() const { return CurrentHitPoints; }

// Methods
public:
	UFUNCTION(BlueprintCallable)
	void StartPreview();

	UFUNCTION(BlueprintCallable)
	void StopPreview();

	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState SetPreviewLocation(FHexagonLocation HexagonLocation);
	
	UFUNCTION(BlueprintCallable)
	static EUnitUpgradeReturnState TryToBuild();

	UFUNCTION(BlueprintCallable)
	bool DeleteExpendedLocation(FHexagonLocation HexagonLocation);

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	FHexagonLocation InitUnitLocation;
	
	UFUNCTION(BlueprintCallable)
	bool CanSpawnInBuilding(ABuilding* Building);

	UFUNCTION(BlueprintCallable)
	bool CanBuildOnLocation(FHexagonLocation HexagonLocation);

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float DamageAmount);
	
	UFUNCTION(BlueprintCallable)
	int GetImproveLevelFromLocation(FHexagonLocation HexagonLocation) const;
	
private:
	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	float CurrentHitPoints;

	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	float CurrentDefence;

	UFUNCTION()
	AUnitView* InitUnitView(FHexagonLocation HexagonLocation);
};
