#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "Field/Unit/UnitView.h"
#include "Player/GamePlayerController.h"
#include "TurnsOrderEventSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStarted);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEnded); // TODO: add delegate with result args
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitViewMove, AUnitView*, UnitView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnitMove, AUnit*, Unit);


UCLASS(BlueprintType)
class TBS_GAME_API UTurnsOrderEventSystem final : public UObject
{
	GENERATED_BODY()
	UTurnsOrderEventSystem() {}

public:
	UPROPERTY(BlueprintAssignable)
	FGameStarted GameStarted;

	UPROPERTY(BlueprintAssignable)
	FUnitViewMove UnitViewPreMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FUnitViewMove UnitViewPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FUnitViewMove UnitViewPostMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FUnitViewMove UnitViewPostMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FUnitMove UnitPreMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FUnitMove UnitPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FUnitMove UnitPostMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FUnitMove UnitPostMoveEnded;
};
