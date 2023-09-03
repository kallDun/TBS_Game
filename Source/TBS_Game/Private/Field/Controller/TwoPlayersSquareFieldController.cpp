#include "Field/Controller/TwoPlayersSquareFieldController.h"
#include "Field/Cell/Cell.h"
#include "Utils/TwoDimArray/CellTwoDimArray.h"


void ATwoPlayersSquareFieldController::InitField_Implementation()
{
	Cells = UCellTwoDimArray::New(Length, Width);
}

void ATwoPlayersSquareFieldController::GenerateField()
{
	Super::GenerateField_Implementation();
	int TerrainTypeIndex = 0;
	InitField();
	for (int i = 0; i < Length; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			const int PcgSeed = FMath::RandRange(-1000000, 1000000);
			const int PlayerOwnerIndex = (i < Length / 2) ? 0 : 1; 
			//SpawnCell(static_cast<ETerrainType>(TerrainTypeIndex), FHexagonLocation(i, j), PlayerOwnerIndex, 0, false, PcgSeed);

			ACell* Cell = GetWorld()->SpawnActor<ACell>(GetCellClassByTerrainType(static_cast<ETerrainType>(TerrainTypeIndex)));
			Cell->Owner = this;
			Cell->Init(this, FHexagonLocation(i, j), PlayerOwnerIndex, 0, false, PcgSeed);
			Cells->SetCell(FHexagonLocation(i, j), Cell);
			
			TerrainTypeIndex = (TerrainTypeIndex + 1) % static_cast<int>(ETerrainType::Count);
		}
	}
}

void ATwoPlayersSquareFieldController::SpawnCell_Implementation(const ETerrainType TerrainType, const FHexagonLocation Location,
                                                                const int PlayerOwner, const int Depth, const bool HasWaterSource, const int PcgSeed)
{
	ACell* Cell = GetWorld()->SpawnActor<ACell>(GetCellClassByTerrainType(TerrainType));
	Cell->Init(this, Location, PlayerOwner, Depth, HasWaterSource, PcgSeed);
	Cells->SetCell(Location, Cell);
}

FHexagonLocation ATwoPlayersSquareFieldController::GetPlayerCenterLocation(const int PlayerIndex) const
{
	FHexagonLocation Center = FHexagonLocation(Length / 2, Width / 2);
	if (PlayerIndex == 0)
	{
		Center.X -= Length / 4;
	}
	else if (PlayerIndex == 1)
	{
		Center.X += Length / 4;
	}
	return Center;
}
