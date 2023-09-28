#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "Field/Utils/ValueProperty.h"
#include "BuildingState.h"
#include "Field/FieldActorsHandler.h"
#include "Field/HexagonLocation.h"
#include "Building.generated.h"

class UCellParamsMapGenerator;
class ABuildingView;
class AGamePlayerController;
enum class EBuildingState : uint8;
enum class EBuildingType : uint8;
enum class EBuildingPlacementReturnState : uint8;
enum class EBuildUpgradeReturnState : uint8;
enum class EBuildingViewState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Building))
class TBS_GAME_API ABuilding : public AFieldActorsHandler
{
	GENERATED_BODY()

public:
	ABuilding() {}

private:
	UFUNCTION(BlueprintCallable)
	void InitUpgradeBuildingComponents(TArray<class UUpgradeBuildingComponent*> Components);

public:
	// components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components", Replicated)
	TArray<UUpgradeBuildingComponent*> UpgradeBuildingComponents = {};
	
	// main properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	EBuildingType BuildingType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	TSubclassOf<ABuildingView> BuildingViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	bool bCanBuildWithOtherBuildings = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = -5, ClampMax = 5, EditCondition = bCanBuildWithOtherBuildings), Category = "Main Properties", Replicated)
	int AffectingOnOtherBuildingImproveLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0f), Category = "Level 1 Properties", Replicated)
	float InitMaxHitPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Level 1 Properties", Replicated)
	int InitMaxCellCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Level 1 Properties", Replicated)
	int MovesToBuild = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10), Category = "Level 1 Properties", Replicated)
	int MovesToAssemble = 0;
	
	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName BuildActionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName ImproveLevelFromLocationInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName PropertyChangedFromImproveLevelInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName RequirementsToExpendCells;
	
	// current state
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	EBuildingState BuildingState = EBuildingState::Initialized;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	int CurrentLevel = 1;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	int MovesToBuildLeft;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	int MovesToAssembleLeft;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	TArray<ABuildingView*> BuildingViews = {};

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	TArray<ABuildingView*> PrefabViews = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	ABuildingView* PrefabPreview;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	FHexagonLocation InitBuildingLocation;

// getters
public:
	virtual TArray<FValueProperty> GetCurrentProperties() const override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FValueProperty> GetPropertiesByLevel(bool UseCurrentLevel = true, int CustomLevel = -1) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHitPoints(bool UseCurrentLevel = true, int CustomLevel = -1) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMaxCellCount(bool UseCurrentLevel = true, int CustomLevel = -1) const;	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalUsedCells() const { return BuildingViews.Num() + PrefabViews.Num(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UUpgradeBuildingComponent*> GetUpgradeComponentsWithLevel(int Level, bool OnlyReadyComponents) const;
	
// Methods
public:
	UFUNCTION(BlueprintCallable)
	void StartPreview();

	UFUNCTION(BlueprintCallable)
	void StopPreview();
	
	UFUNCTION(BlueprintCallable)
	EBuildingPlacementReturnState SetPreviewLocation(FHexagonLocation HexagonLocation);
	
	UFUNCTION(BlueprintCallable)
	EBuildingPlacementReturnState TryToExpendLocation(FHexagonLocation HexagonLocation);
	
	UFUNCTION(BlueprintCallable)
	bool DeleteExpendedLocation(FHexagonLocation HexagonLocation);
	
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToBuild();
	
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToContinueBuilding();
	
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToUpgrade();

	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToContinueUpgrading();

	UFUNCTION(BlueprintCallable)
	int GetImproveLevelFromLocation(FHexagonLocation HexagonLocation) const;

	UFUNCTION(BlueprintCallable)
	bool CanBuildOnLocation(FHexagonLocation HexagonLocation) const;

	UFUNCTION(BlueprintCallable)
	bool CanExpendLocation(FHexagonLocation HexagonLocation) const;

	UFUNCTION()
	void ChangeBuildingViewsState(EBuildingViewState NewState);

// Move tick
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartPreMoveTick();

	UFUNCTION()
	void StartBuildingViewPreMove(ABuildingView* BuildingView);

	UFUNCTION()
	void EndBuildingViewPreMove(ABuildingView* BuildingView);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartPostMoveTick();

	UFUNCTION()
	void StartBuildingViewPostMove(ABuildingView* BuildingView);

	UFUNCTION()
	void EndBuildingViewPostMove(ABuildingView* BuildingView);

	UFUNCTION()
	void AssembleMoveTick();

private:
	UFUNCTION()
	ABuildingView* InitBuildingView(FHexagonLocation HexagonLocation, const bool bIsMainView);
	
	UFUNCTION()
	void BuildAllPrefabViews();
	
// BP events
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanBuild() const;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangePropertiesAccordingToImproveLevel(int ImproveLevel); // TODO: add by default event to increase moves to build
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBuildAction();
	
};
