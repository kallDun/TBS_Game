#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "Field/Unit/UnitView.h"
#include "Player/GamePlayerController.h"
#include "TurnsOrderEventSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStarted);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameEnded); // TODO: add delegate with result args
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerInitialize, AGamePlayerController*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTurn, AGamePlayerController*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerTurnChanged, AGamePlayerController*, Player, EPlayerTurnType, TurnType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerUnitsMove, AGamePlayerController*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMove, AGamePlayerController*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildingViewMove, ABuildingView*, BuildingView);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildingMove, ABuilding*, Building);
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
	FPlayerInitialize PlayerInitializeFinished;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerTurn PlayerTurnStarted;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerTurnChanged PlayerTurnTypeChanged;	

	UPROPERTY(BlueprintAssignable)
	FPlayerTurn PlayerTurnEnded;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerUnitsMove PlayerUnitsMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FPlayerUnitsMove PlayerUnitsMoveEnded;

	UPROPERTY(BlueprintAssignable)
	FPlayerMove PlayerMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FPlayerMove PlayerMoveEnded;

	UPROPERTY(BlueprintAssignable)
	FBuildingViewMove BuildingViewPreMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FBuildingViewMove BuildingViewPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingViewMove BuildingViewPostMoveStarted;

	UPROPERTY(BlueprintAssignable)
	FBuildingViewMove BuildingViewPostMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingMove BuildingPreMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingMove BuildingPreMoveEnded;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingMove BuildingPostMoveStarted;
	
	UPROPERTY(BlueprintAssignable)
	FBuildingMove BuildingPostMoveEnded;

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
