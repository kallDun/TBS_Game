#pragma once

#include "CoreMinimal.h"
#include "Ability.generated.h"

USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealingAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementModifier;
};

