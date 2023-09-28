#include "Field/FieldActorsHandler.h"
#include "Utils/TwoDimArray/CellParamsTwoDimArray.h"
#include <Engine/ActorChannel.h>
#include <Net/UnrealNetwork.h>


void AFieldActorsHandler::Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner)
{
	PlayerControllerRef = PlayerControllerOwner;
	AGameActor::Init(Field);
}


TArray<FValueProperty> AFieldActorsHandler::GetCurrentProperties() const
{
	return InitProperties;
}

float AFieldActorsHandler::GetPropertyValue(const FName PropertyName, const float ValueByDefault) const
{
	TArray<FValueProperty> Properties = GetCurrentProperties();
	for (auto Property : Properties)
	{
		if (Property.Name == PropertyName)
		{
			return Property.Value;
		}
	}
	return ValueByDefault;
}

bool AFieldActorsHandler::SetPropertyValue(const FName PropertyName, const float Value) const
{
	for (auto Property : InitProperties)
	{
		if (Property.Name == PropertyName)
		{
			Property.SetValue(Value);
			return true;
		}
	}
	return false;
}

void AFieldActorsHandler::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Main properties
	DOREPLIFETIME( AFieldActorsHandler, Name );
	DOREPLIFETIME( AFieldActorsHandler, InitProperties );
	DOREPLIFETIME( AFieldActorsHandler, AnchorPoints );
	DOREPLIFETIME( AFieldActorsHandler, TerrainRules );
	DOREPLIFETIME( AFieldActorsHandler, NecessaryCellSpace );
	// Info properties
	DOREPLIFETIME( AFieldActorsHandler, IconMedium );
	DOREPLIFETIME( AFieldActorsHandler, Description );
	DOREPLIFETIME( AFieldActorsHandler, LocationRequirementsInfo );
	DOREPLIFETIME( AFieldActorsHandler, MainRequirementsInfo );
	// State properties
	DOREPLIFETIME( AFieldActorsHandler, PlayerControllerRef );
	DOREPLIFETIME( AFieldActorsHandler, CellParamsMap );
}

bool AFieldActorsHandler::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bFlag = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bFlag |= Channel->ReplicateSubobject(CellParamsMap, *Bunch, *RepFlags);
	return bFlag;
}
