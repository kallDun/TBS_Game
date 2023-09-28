#pragma once
#include "AnchorRadiusType.h"
#include "AnchorPointType.h"
#include "Field/HexagonLocation.h"
#include "SingleAnchorData.h"
#include "AnchorPoint.generated.h"

class AFieldActorsHandler;
class ABuilding;
class UBuildingBehaviourComponent;

USTRUCT(BlueprintType)
struct FAnchorPoint
{
	GENERATED_BODY()

public:
	FAnchorPoint() = default;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAnchorPointType PointType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "PointType == EAnchorPointType::Fixed"))
	FHexagonLocation FixedLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "PointType == EAnchorPointType::BehaviourComponent"))
	TSubclassOf<UBuildingBehaviourComponent> BuildingBehaviourClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "PointType == EAnchorPointType::BuildingName"))
	FName BuildingName;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAnchorRadiusType RadiusType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1, EditConditionHides, EditCondition = "RadiusType == EAnchorRadiusType::Fixed"))
	int FixedRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditConditionHides, EditCondition = "(RadiusType == EAnchorRadiusType::CurrentBuildingProperty) || (RadiusType == EAnchorRadiusType::AnchorBuildingProperty && (PointType == EAnchorPointType::BehaviourComponent || PointType == EAnchorPointType::BuildingName))"))
	FName RadiusBuildingPropertyName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = -10, ClampMax = 0))
	int MaximumDebuffLevelToBuild = -10;
	
// functions
	TArray<FSingleAnchorData> GetAnchors(const AFieldActorsHandler* Actor, const int DefaultRadius) const;
};
