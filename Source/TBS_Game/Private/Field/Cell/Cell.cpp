#include "Field/Cell/Cell.h"
#include <Net/UnrealNetwork.h>


ACell::ACell()
{
	bReplicates = true;
	AActor::SetReplicateMovement(true);
}

void ACell::Init(AFieldController* Field, const FHexagonLocation InLocation, const int PlayerOwner, const int InDepth, const bool InHasWaterSource, const int PcgSeed)
{
	AGameActor::Init(Field);
	PCGSeed = PcgSeed;
	PlayerOwnerNumber = PlayerOwner;
	Location = InLocation;
	Depth = InDepth;
	bHasWaterSource = InHasWaterSource;
	UpdateWorldLocation();
}

void ACell::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACell, Type);
	DOREPLIFETIME(ACell, PlayerOwnerNumber);
	DOREPLIFETIME(ACell, Depth);
	DOREPLIFETIME(ACell, bHasWaterSource);
	DOREPLIFETIME(ACell, PCGSeed);
}
