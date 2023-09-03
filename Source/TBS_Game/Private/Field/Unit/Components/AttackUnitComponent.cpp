#include "Field/Unit/Components/AttackUnitComponent.h"

#include "Field/Unit/Unit.h"
#include "Field/Unit/UnitState.h"

float UAttackUnitComponent::CalculateDamage(const UAttackUnitComponent* Attacker, const AUnit* Target)
{
	float baseDamage = Attacker->GetDamage(); // Replace this with your logic to get the attack damage of the attacker
	float defence = Target->GetMaxDefence(); // Replace this with your logic to get the defense of the target
    
	float damageDealt = FMath::Max(0.0f, baseDamage - defence); // Ensure damage dealt is non-negative
	return damageDealt;
}

void UAttackUnitComponent::AttackUnit(AUnit* Attacker, AUnit* TargetUnit)
{
	if (Attacker->GetUnitState() == EUnitState::Initialized && TargetUnit->GetUnitState() != EUnitState::Destroyed)
	{
		float damageDealt = CalculateDamage(this, TargetUnit);
		//TargetUnit->ReceiveDamage(damageDealt);
		// Add any other combat-related logic here
	}
}
