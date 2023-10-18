#pragma once
#include "CoreMinimal.h"
#include "UnitBehaviourComponent.h"
#include "AttackUnitComponent.generated.h"

class AUnit;
class ACell;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TBS_GAME_API UAttackUnitComponent : public UUnitBehaviourComponent
{
	GENERATED_BODY()

public:
	UAttackUnitComponent() {}

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (AllowPrivateAccess = "true"), Category = "Attack")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Attack")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Attack")
	bool bCanSplash;

//Getters	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamage() const { return Damage; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRange() const { return Range; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCanSplash() const { return bCanSplash; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float CalculateDamage(const UAttackUnitComponent* Attacker, const AUnit* Target);

	UFUNCTION(BlueprintCallable)
	void AttackUnit(AUnit* Attacker, AUnit* TargetUnit);
};
