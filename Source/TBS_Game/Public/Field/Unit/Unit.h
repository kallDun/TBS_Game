#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "UnitView.h"
#include "Field/FieldActorsHandler.h"
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
enum class EUnitPlaceReturnState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Unit))
class TBS_GAME_API AUnit : public AFieldActorsHandler
{
	GENERATED_BODY()

public:
	AUnit() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	EUnitPillarType UnitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	EUnitAttackType UnitTypeByPillar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Main Properties", Replicated)
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	TSubclassOf<AUnitView> UnitViewClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	float InitMaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Properties", Replicated)
	float InitMaxDefence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.5", ClampMax = "5"), Category = "Main Properties", Replicated)
	float InitMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "10"), Category = "Main Properties", Replicated)
	int MovesToAssemble = 0;

	// current state
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	AUnitView* PrefabPreview;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	TArray<AUnitView*> UnitViews = {};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	int AvailableUnitsCount = 0;

// Methods
public:
	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState SetPreviewLocation(FHexagonLocation HexagonLocation);

	UFUNCTION(BlueprintCallable)
	void StartPreview();
	
	UFUNCTION(BlueprintCallable)
	void StopPreview();

	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState CheckUnitPlacement();

	UFUNCTION(BlueprintCallable)
	EUnitPlacementReturnState TryToPlace();

	UFUNCTION(BlueprintImplementableEvent)
	bool CanPlace();

// Player move
public:
	UFUNCTION()
	void AssembleMoveTick();

	UFUNCTION()
	void StartMoveTick();

	UFUNCTION()
	void EndUnitViewMoveTick(AUnitView* UnitView);

private:
	UFUNCTION()
	void EndMoveTick();
	
	UFUNCTION()
	void StartUnitViewMoveTick(AUnitView* UnitView);

	UFUNCTION()
	void AssembleUnitViewMoveTick(AUnitView* UnitView);

// Auxiliary
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
