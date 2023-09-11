#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "BuildingProperty.h"
#include "BuildingState.h"
#include "Field/GameActor.h"
#include "Field/HexagonLocation.h"
#include "Field/Anchor/AnchorPoint.h"
#include "Field/Anchor/TerrainRules.h"
#include "Building.generated.h"

class UCellParamsMap;
class ABuildingView;
class AGamePlayerController;
enum class EBuildingState : uint8;
enum class EBuildingType : uint8;
enum class EBuildingPlacementReturnState : uint8;
enum class EBuildUpgradeReturnState : uint8;
enum class EBuildingViewState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildingViewEvent, ABuildingView*, BuildingView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuildingEvent);

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Building))
class TBS_GAME_API ABuilding : public AGameActor
{
	GENERATED_BODY()

public:
	ABuilding()
	{
		bReplicates = true;
		AActor::SetReplicateMovement(true);
	}
	void Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner);

public:
	// components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components", Replicated)
	TArray<class UUpgradeBuildingComponent*> UpgradeBuildingComponents = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	FName BuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	EBuildingType BuildingType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	TSubclassOf<ABuildingView> BuildingViewClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	USceneComponent* BuildingMeshRef = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	TArray<FBuildingProperty> InitProperties = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	bool bCanBuildWithOtherBuildings = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = -5, ClampMax = 5, EditCondition = bCanBuildWithOtherBuildings), Category = "Building Main", Replicated)
	int AffectingOnOtherBuildingImproveLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	TArray<FAnchorPoint> AnchorPoints = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Building Main", Replicated)
	FTerrainRules TerrainRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0f), Category = "Level 1", Replicated)
	float InitMaxHitPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Level 1", Replicated)
	int InitMaxCellCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Category = "Level 1", Replicated)
	int MovesToBuild = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10), Category = "Level 1", Replicated)
	int MovesToAssemble = 0;
	
	// info properties	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	UTexture2D* BuildingIconMedium;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName BuildingDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName BuildActionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName ImproveLevelFromLocationInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName PropertyChangedFromImproveLevelInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName LocationRequirementsToBuildInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName MainRequirementsToBuildInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName RequirementsToExpendCells;
	
	// current state
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	AGamePlayerController* PlayerControllerRef = nullptr;

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
	UCellParamsMap* CellParamsMap = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	FHexagonLocation InitBuildingLocation;

	// events
	UPROPERTY(BlueprintAssignable)
	FBuildingViewEvent BuildingViewPreMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FBuildingViewEvent BuildingViewPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingViewEvent BuildingViewPostMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FBuildingViewEvent BuildingViewPostMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingEvent BuildingPreMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingEvent BuildingPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingEvent BuildingPostMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingEvent BuildingPostMoveEnded;

// getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FBuildingProperty> GetProperties(bool UseCurrentLevel = true, int CustomLevel = -1) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHitPoints(bool UseCurrentLevel = true, int CustomLevel = -1) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetMaxCellCount(bool UseCurrentLevel = true, int CustomLevel = -1) const;	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalUsedCells() const { return BuildingViews.Num() + PrefabViews.Num(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPropertyValue(FName PropertyName, float ValueByDefault) const;
	
	UFUNCTION(BlueprintCallable)
	bool SetPropertyValue(FName PropertyName, float Value) const;
	
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PrePlayerMoveTick();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PostPlayerMoveTick();

	UFUNCTION()
	void AssembleMoveTick();

	UFUNCTION()
	void ChangeBuildingViewsState(EBuildingViewState NewState);
	
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

// turns order
private:
	UPROPERTY()
	int BuildingViewMoveCalledCount = 0;

	UFUNCTION()
	void DoNextBuildingViewMove(bool bIsPreMove);
	
	UFUNCTION()
	void BuildingViewPreMoveEndedEventHandler(ABuildingView* BuildingView);
	
	UFUNCTION()
	void BuildingViewPostMoveEndedEventHandler(ABuildingView* BuildingView);
};
