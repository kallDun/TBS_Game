#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitBehaviourComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UUnitBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUnitBehaviourComponent() {}

public:
	UFUNCTION(BlueprintCallable)
	virtual void UnitMoveTick();

protected:	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnitMoveTickBP();
};
