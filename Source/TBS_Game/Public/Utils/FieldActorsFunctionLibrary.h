#pragma once
#include "CoreMinimal.h"
#include "Field/HexagonLocation.h"
#include "FieldActorsFunctionLibrary.generated.h"

class AFieldActor;
class ABuilding;
class UBuildingBehaviourComponent;
class ABuildingView;

UCLASS()
class TBS_GAME_API UFieldActorsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static TArray<ABuildingView*> GetBuildingViewsByBehaviourComponent(const UObject* WorldContextObject,const TSubclassOf<UBuildingBehaviourComponent> ComponentClass,
		const bool bIncludePreviewActors = false, const bool bIncludeNotBuiltActors = true, const ABuilding* NotIncludedBuilding = nullptr);

	UFUNCTION(BlueprintCallable)
	static TArray<ABuildingView*> GetBuildingViewsByBuildingName(const UObject* WorldContextObject,const FName BuildingName,
		const bool bIncludePreviewActors = false, const bool bIncludeNotBuiltActors = true);

	UFUNCTION(BlueprintCallable)
	static TArray<AFieldActor*> GetFieldActorsByLocation(const UObject* WorldContextObject,const FHexagonLocation Location,
		const bool bIncludePreviewActors = false, const bool bIncludeNotBuiltActors = true);
};
