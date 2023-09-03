// Fill out your copyright notice in the Description page of Project Settings.

#include "Field/Unit/Components/MovementUnitComponent.h"

#include "Field/HexagonLocation.h"

void UMovementUnitComponent::UnitMoveTick_Implementation()
{
}

bool UMovementUnitComponent::TryToPass(FHexagonLocation HexagonLocation)
{
	return true;
}
