#pragma once
#include "CoreMinimal.h"
#include "TerrainType.h"
#include "Field/FieldActor.h"
#include "Cell.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Cell))
class TBS_GAME_API ACell : public AFieldActor
{
	GENERATED_BODY()

public:
	ACell() 
	{
		bReplicates = true;
		SetReplicateMovement(true);
	}
	void Init(AFieldController* Field, const FHexagonLocation Location, const int PlayerOwner,
		const int Depth, const bool bHasWaterSource, const int PcgSeed);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCellInit();

// Components
protected:	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	int PCGSeed;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter=GetTerrainType, meta = (AllowPrivateAccess = "true"), Replicated)
	ETerrainType Type;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetPlayerOwnerNumber, meta = (AllowPrivateAccess = "true"), Replicated)
	int PlayerOwnerNumber;
	
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetDepth, meta = (AllowPrivateAccess = "true"), Replicated)
	int Depth; // positive is above ground, negative is below ground

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	bool bHasWaterSource;

// TODO: add getters for properties if needed
public:
	UFUNCTION(BlueprintGetter)
	ETerrainType GetTerrainType() const { return Type; }
	
	UFUNCTION(BlueprintGetter)
	int GetPlayerOwnerNumber() const { return PlayerOwnerNumber; }
	
	UFUNCTION(BlueprintGetter)
	int GetDepth() const { return Depth; }

// Methods
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
