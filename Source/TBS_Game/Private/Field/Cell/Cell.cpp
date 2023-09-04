#include "Field/Cell/Cell.h"
#include <Net/UnrealNetwork.h>


void ACell::Init(AFieldController* Field, const FHexagonLocation InitLocation, const int PlayerOwner, const int InitDepth, const bool HasWaterSource, const int PcgSeed)
{
	AGameActor::Init(Field);
	PCGSeed = PcgSeed;
	PlayerOwnerNumber = PlayerOwner;
	Location = InitLocation;
	Depth = InitDepth;
	bHasWaterSource = HasWaterSource;
	UpdateWorldLocation();
	OnCellInit();
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
