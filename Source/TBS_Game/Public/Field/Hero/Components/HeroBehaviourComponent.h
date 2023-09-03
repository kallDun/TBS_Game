#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeroBehaviourComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UHeroBehaviourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHeroBehaviourComponent() {}

public:
	UFUNCTION(BlueprintCallable)
	virtual void HeroMoveTick();
	
};
