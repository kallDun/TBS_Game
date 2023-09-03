#include "Field/Building/Components/BuildingBehaviourComponent.h"


void UBuildingBehaviourComponent::PrePlayerMoveTick()
{
	OnPrePlayerMoveTick();
}

void UBuildingBehaviourComponent::PostPlayerMoveTick()
{
	OnPostPlayerMoveTick();
}
