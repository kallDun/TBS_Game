#pragma once
#include "CoreMinimal.h"
#include "BuildingBehaviourComponent.h"
#include "MainAnchorBehaviourComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Building), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API UMainAnchorBehaviourComponent : public UBuildingBehaviourComponent
{
	GENERATED_BODY()

public:
	UMainAnchorBehaviourComponent() {}

};
