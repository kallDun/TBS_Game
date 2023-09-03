#pragma once

#include "CoreMinimal.h"
#include "Utils/Ability.h"
#include "UnitBehaviourComponent.h"
#include "Components/ActorComponent.h"
#include "UnitEnhancementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UUnitEnhancementComponent : public UUnitBehaviourComponent
{
	GENERATED_BODY()

public:
	UUnitEnhancementComponent()
	{
		// Example
		FAbility HealingAbility;
		HealingAbility.AbilityName = FName("Healing");
		HealingAbility.AbilityDescription = FName("Restores health to the unit");
		HealingAbility.HealingAmount = 50.0f;

		FAbility AttackModifierAbility;
		AttackModifierAbility.AbilityName = FName("Attack Boost");
		AttackModifierAbility.AbilityDescription = FName("Increases the unit's attack power");
		AttackModifierAbility.AttackModifier = 1.5f;

		// Adding abilities to the array
		Abilities.Add(HealingAbility);
		Abilities.Add(AttackModifierAbility);
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Enhance")
	TArray<FAbility> Abilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Enhance")
	bool bCanNeutralizeEnhance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Enhance")
	int ReligiousFollowersIncreasePerTurn;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FAbility> GetAbilities() const { return Abilities; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanNeutralizeEnhance() const { return bCanNeutralizeEnhance; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetReligiousFollowersIncreasePerTurn() const { return ReligiousFollowersIncreasePerTurn; }

	UFUNCTION(BlueprintCallable)
	void AddAbility(const FAbility& Ability)
	{
		Abilities.Add(Ability);
	}
};