#pragma once
#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "Building.h"
#include "UpgradingState.h"
#include "UpgradeBuildingComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Building), meta = (BlueprintSpawnableComponent))
class TBS_GAME_API UUpgradeBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUpgradeBuildingComponent() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = 2, ClampMax = 10), Replicated)
	int LevelNumber = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0f), Replicated)
	float MaxHitPointsAddition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10), Replicated)
	int MaxCellCountAddition = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 1, ClampMax = 10), Replicated)
	int MovesToBuild = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 10), Replicated)
	int MovesToAssemble = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	TArray<FValueProperty> Properties = {};
	
	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName UpgradeActionInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName PropertyChangedFromImproveLevelInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Info", Replicated)
	FName MainRequirementsToBuildInfo;
	
	// additional properties
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties", Replicated)
	EUpgradingState UpgradingState = EUpgradingState::Initialized;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties", Replicated)
	int MovesToBuildLeft;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Properties", Replicated)
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangePropertiesAccordingToImproveLevel(int ImproveLevel); // TODO: add by default event to increase moves to build
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUpgradeAction();
};
