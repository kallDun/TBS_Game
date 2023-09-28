#pragma once
#include "CoreMinimal.h"
#include "GameActor.h"
#include "Field/Utils/ValueProperty.h"
#include "Field/Anchor/AnchorPoint.h"
#include "Field/Anchor/TerrainRules.h"
#include "FieldActorsHandler.generated.h"

class AGamePlayerController;

UCLASS()
class TBS_GAME_API AFieldActorsHandler : public AGameActor
{
	GENERATED_BODY()

public:
	AFieldActorsHandler()
	{
		bReplicates = true;
		AActor::SetReplicateMovement(true);
	}

	UFUNCTION()
	void Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner);

public:
	// main properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	TArray<FValueProperty> InitProperties = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	TArray<FAnchorPoint> AnchorPoints = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Main Properties", Replicated)
	FTerrainRules TerrainRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = 0.f, ClampMax = 1.f), Category = "Main Properties", Replicated)
	float NecessaryCellSpace = .5f;

	// info properties	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	UTexture2D* IconMedium;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName LocationRequirementsInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Info Properties", Replicated)
	FName MainRequirementsInfo;
	
	// current state
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	AGamePlayerController* PlayerControllerRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	class UCellParamsTwoDimArray* CellParamsMap = nullptr;

	
// getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual TArray<FValueProperty> GetCurrentProperties() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPropertyValue(FName PropertyName, float ValueByDefault) const;
	
	UFUNCTION(BlueprintCallable)
	bool SetPropertyValue(FName PropertyName, float Value) const;

// Network
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool ReplicateSubobjects(UActorChannel *Channel, FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

};
