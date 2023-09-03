#pragma once
#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "Building.h"
#include "BuildingProperty.h"
#include "UpgradingState.h"
#include "UpgradeBuildingComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Building), meta = (BlueprintSpawnableComponent))
class TBS_GAME_API UUpgradeBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUpgradeBuildingComponent() {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = 2, ClampMax = 10))
	int LevelNumber = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0f))
	float MaxHitPointsAddition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10))
	int MaxCellCountAddition = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10))
	int MovesToBuild = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10))
	int MovesToAssemble = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FBuildingProperty> Properties = {};
	
	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info")
	FName UpgradeActionInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info")
	FName PropertyChangedFromImproveLevelInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info")
	FName MainRequirementsToBuildInfo;
	
	// additional properties
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	EUpgradingState UpgradingState = EUpgradingState::Initialized;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	int MovesToBuildLeft;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	int MovesToAssembleLeft;

	
// getters & setters
public:
	UFUNCTION(BlueprintGetter)
	ABuilding* GetBuildingRef() const { return static_cast<ABuilding*>(GetOwner()); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPropertyValue(FName PropertyName, float ValueByDefault) const;
	
	UFUNCTION(BlueprintCallable)
	bool SetPropertyValue(FName PropertyName, float Value) const;


// Methods
public:
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToUpgrade();

	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState TryToContinueUpgrading();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PrePlayerMoveTick();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PostPlayerMoveTick();

	UFUNCTION()
	void AssembleMoveTick();

	
// Blueprint events
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool CanUpgrade() const;
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangePropertiesAccordingToImproveLevel(int ImproveLevel); // TODO: add by default event to increase moves to build
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUpgradeAction();
};
