#include "Field/Anchor/AnchorPoint.h"
#include "Field/Building/Building.h"
#include "Field/Building/BuildingView.h"
#include "Player/GamePlayerController.h"
#include "Utils/FieldActorsFunctionLibrary.h"


TArray<FSingleAnchorData> FAnchorPoint::GetAnchors(const ABuilding* Building, const int DefaultRadius) const
{
	TArray<FSingleAnchorData> Anchors = {};
	if (PointType == EAnchorPointType::Fixed ||
		PointType == EAnchorPointType::PlayerInitialPosition)
	{
		int Radius = DefaultRadius;		
		if (RadiusType == EAnchorRadiusType::Fixed)
		{
			Radius = FixedRadius;
		}
		else if (RadiusType == EAnchorRadiusType::CurrentBuildingProperty)
		{
			const float RadiusFloat = Building->GetPropertyValue(RadiusBuildingPropertyName, DefaultRadius);
			Radius = FMath::RoundToInt(RadiusFloat);
		}
		const FHexagonLocation Location = PointType == EAnchorPointType::Fixed
			? FixedLocation : Building->PlayerControllerRef->CenterLocation;
		Anchors.Add(FSingleAnchorData(Location, Radius, MaximumDebuffLevelToBuild));
	}
	else if (PointType == EAnchorPointType::BehaviourComponent
		|| PointType == EAnchorPointType::BuildingName)
	{
		TArray<ABuildingView*> BuildingViews = PointType == EAnchorPointType::BehaviourComponent
			? UFieldActorsFunctionLibrary::GetBuildingViewsByBehaviourComponent(Building, BuildingBehaviourClass, false, false, Building)
			: UFieldActorsFunctionLibrary::GetBuildingViewsByBuildingName(Building, BuildingName, false, false);
		
		for (const ABuildingView* BuildingView : BuildingViews)
		{
			int Radius = DefaultRadius;		
			if (RadiusType == EAnchorRadiusType::Fixed)
			{
				Radius = FixedRadius;
			}
			else if (RadiusType == EAnchorRadiusType::CurrentBuildingProperty)
			{
				Radius = FMath::RoundToInt(Building->GetPropertyValue(RadiusBuildingPropertyName, DefaultRadius));
			}
			else if (RadiusType == EAnchorRadiusType::AnchorBuildingProperty)
			{
				Radius = FMath::RoundToInt(BuildingView->GetBuildingRef()->GetPropertyValue(RadiusBuildingPropertyName, DefaultRadius));
			}
			Anchors.Add(FSingleAnchorData(BuildingView->GetLocation(), Radius, MaximumDebuffLevelToBuild));
		}
	}

	return Anchors;
}
