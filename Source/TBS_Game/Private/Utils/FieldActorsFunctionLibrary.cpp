#include "Utils/FieldActorsFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Field/Building/Building.h"
#include "Field/Building/BuildingView.h"
#include "Field/Building/Components/BuildingBehaviourComponent.h"


TArray<ABuildingView*> UFieldActorsFunctionLibrary::GetBuildingViewsByBehaviourComponent(
	const UObject* WorldContextObject, const TSubclassOf<UBuildingBehaviourComponent> ComponentClass,
	const bool bIncludePreviewActors, const bool bIncludeNotBuiltActors, const ABuilding* NotIncludedBuilding)
{
	TArray<AActor*> BuildingViewsActors = {};
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject->GetWorld(), ABuildingView::StaticClass(), BuildingViewsActors);
	
	TArray<ABuildingView*> BuildingViews = {};
	for (AActor* const BuildingViewsActor : BuildingViewsActors)
	{
		if (ABuildingView* BuildingView = Cast<ABuildingView>(BuildingViewsActor))
		{
			if (UActorComponent* Component = BuildingView->GetComponentByClass(ComponentClass))
			{
				if (bIncludePreviewActors || !BuildingView->IsPreviewState())
				{
					if (bIncludeNotBuiltActors || !BuildingView->IsNotBuiltState())
					{
						if (NotIncludedBuilding == nullptr || NotIncludedBuilding != BuildingView->GetBuildingRef())
						{
							BuildingViews.Add(BuildingView);
						}
					}
				}
			}
		}
	}
	return BuildingViews;
}

TArray<ABuildingView*> UFieldActorsFunctionLibrary::GetBuildingViewsByBuildingName(const UObject* WorldContextObject,const FName BuildingName,
	const bool bIncludePreviewActors, const bool bIncludeNotBuiltActors)
{
	TArray<AActor*> BuildingViewsActors = {};
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject->GetWorld(), ABuildingView::StaticClass(), BuildingViewsActors);

	TArray<ABuildingView*> BuildingViews = {};
	for (AActor* const BuildingViewsActor : BuildingViewsActors)
	{
		if (ABuildingView* BuildingView = Cast<ABuildingView>(BuildingViewsActor))
		{
			if (BuildingView->GetBuildingRef()->Name == BuildingName)
			{
				if (bIncludePreviewActors || !BuildingView->IsPreviewState())
				{
					if (bIncludeNotBuiltActors || !BuildingView->IsNotBuiltState())
					{
						BuildingViews.Add(BuildingView);
					}
				}
			}
		}
	}
	return BuildingViews;
}

TArray<AFieldActor*> UFieldActorsFunctionLibrary::GetFieldActorsByLocation(const UObject* WorldContextObject, const FHexagonLocation Location,
	const bool bIncludePreviewActors, const bool bIncludeNotBuiltActors)
{
	TArray<AActor*> Actors = {};
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject->GetWorld(), AFieldActor::StaticClass(), Actors);

	TArray<AFieldActor*> FieldActors = {};
	for (AActor* const Actor : Actors)
	{
		if (AFieldActor* FieldActor = Cast<AFieldActor>(Actor))
		{
			if (FieldActor->GetLocation() == Location)
			{
				if (bIncludePreviewActors || !FieldActor->IsPreviewState())
				{
					if (bIncludeNotBuiltActors || !FieldActor->IsNotBuiltState())
					{
						FieldActors.Add(FieldActor);
					}
				}
			}
		}
	}
	return FieldActors;
}
