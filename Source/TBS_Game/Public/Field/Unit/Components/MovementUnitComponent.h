// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovementUnitComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UMovementUnitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovementUnitComponent() {}

protected:

public:
	UFUNCTION(BlueprintCallable)
	bool TryToPass(FHexagonLocation HexagonLocation); // TODO: add argument for terrain type or enum to check what to do

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnitMoveTick();
};
