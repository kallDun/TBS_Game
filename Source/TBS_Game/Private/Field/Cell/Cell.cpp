#include "Field/Cell/Cell.h"


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
