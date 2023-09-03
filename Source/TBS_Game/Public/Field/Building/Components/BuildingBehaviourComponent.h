#pragma once
#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "BuildingBehaviourComponent.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Building), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UBuildingBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildingBehaviourComponent() {}

public:
	UFUNCTION(BlueprintCallable)
	virtual void PrePlayerMoveTick();
	
	UFUNCTION(BlueprintCallable)
	virtual void PostPlayerMoveTick();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPrePlayerMoveTick();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPostPlayerMoveTick();
	
};
