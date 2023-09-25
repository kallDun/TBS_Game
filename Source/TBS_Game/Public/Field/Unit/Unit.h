#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "Field/FieldActor.h"
#include "Field/Anchor/TerrainRules.h"
#include "Field/Utils/ValueProperty.h"
#include "Field/Anchor/AnchorPoint.h"
#include "UnitView.h"
#include "Unit.generated.h"

class ABuildingView;
class ACell;
class ABuilding;
class AUnitView;
class UCellParamsMapGenerator;
class AGamePlayerController;
enum class EUnitPillarType : uint8;
enum class EUnitAttackType : uint8;
enum class EUnitPlacementReturnState : uint8;
enum class EUnitUpgradeReturnState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Unit))
class TBS_GAME_API AUnit : public AGameActor
{
	GENERATED_BODY()

public:
	AUnit()
	{
		bReplicates = true;
		AActor::SetReplicateMovement(true);
	}
	void Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main", Replicated)
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main", Replicated)
	EUnitPillarType UnitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Unit Main", Replicated)
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Main", Replicated)
	TSubclassOf<AUnitView> UnitViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	TArray<FValueProperty> InitProperties = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	TArray<FAnchorPoint> AnchorPoints = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Main", Replicated)
	FTerrainRules TerrainRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0.f, ClampMax = 1.f), Category = "Unit Main", Replicated)
	float NecessaryCellSpace = .25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Main", Replicated)
	float InitMaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Main", Replicated)
	float InitMaxDefence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.5", ClampMax = "5"), Category = "Unit Main", Replicated)
	float InitMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "10"), Category = "Unit Main", Replicated)
	int MovesToAssemble = 0;

	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Unit Info", Replicated)
	UTexture2D* UnitIconMedium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info", Replicated)
	FName UnitDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info", Replicated)
	FName LocationRequirementsToSpawnInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Unit Info", Replicated)
	FName MainRequirementsToSpawnInfo;

	// current state
	UPROPERTY(BlueprintGetter=GetPlayerControllerRef, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	AGamePlayerController* PlayerControllerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	AUnitView* PrefabPreview;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	TArray<AUnitView*> UnitViews = {};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	class UCellParamsTwoDimArray* CellParamsMap = nullptr;

	
// Getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGamePlayerController* GetPlayerControllerRef() const { return PlayerControllerRef; }

// Methods
public:
	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState SetPreviewLocation(FHexagonLocation HexagonLocation);

	UFUNCTION(BlueprintCallable)
	void StartPreview();
	
	UFUNCTION(BlueprintCallable)
	void StopPreview();

	UFUNCTION(BlueprintCallable)
	EUnitUpgradeReturnState TryToPlace();

	UFUNCTION(BlueprintImplementableEvent)
	bool CanPlace();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	bool CanPlaceOnLocation(FHexagonLocation HexagonLocation) const;

private:
	UFUNCTION()
	AUnitView* InitUnitView(FHexagonLocation HexagonLocation);

	UFUNCTION()
	void PlacePrefabView();
	
};
