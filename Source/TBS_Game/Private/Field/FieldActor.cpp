#include "Field/FieldActor.h"
#include "Field/Controller/FieldController.h"
#include "Utils/HexagonFunctionLibrary.h"
#include <Net/UnrealNetwork.h>


void AFieldActor::SetAndUpdateLocation(const FHexagonLocation NewLocation)
{
	Location = NewLocation;
	UpdateWorldLocation();
}

void AFieldActor::UpdateWorldLocation()
{
	const FVector WorldLocation = UHexagonFunctionLibrary::GetWorldLocationFromHexagon(Location,
		FieldController->GetHexagonSize(), FieldController->GetFieldCenter());
	SetActorLocation(WorldLocation);
}

void AFieldActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFieldActor, Location);
}
