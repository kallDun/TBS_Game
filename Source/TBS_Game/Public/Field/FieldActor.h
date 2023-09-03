#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "GameActor.h"
#include "HexagonLocation.h"
#include "FieldActor.generated.h"

class ATBS_GameModeBase;

UCLASS(Abstract, BlueprintType)
class TBS_GAME_API AFieldActor : public AGameActor
{
	GENERATED_BODY()

public:
	AFieldActor() {}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Getter = GetLocation, meta = (AllowPrivateAccess = "true"))
	FHexagonLocation Location;

// Getters
public:
	UFUNCTION(BlueprintGetter)
	FHexagonLocation GetLocation() const { return Location; }
	
	UFUNCTION(BlueprintGetter)
	virtual bool IsPreviewState() const { return false; }

	UFUNCTION(BlueprintGetter)
	virtual bool IsNotBuiltState() const { return false; }

// Methods
public:
	UFUNCTION(BlueprintCallable)
	void SetAndUpdateLocation(FHexagonLocation NewLocation);

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateWorldLocation();
};
