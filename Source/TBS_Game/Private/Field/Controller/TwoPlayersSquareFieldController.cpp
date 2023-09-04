#include "Field/Controller/TwoPlayersSquareFieldController.h"
#include "Field/Cell/Cell.h"
#include "Utils/TwoDimArray/CellTwoDimArray.h"


void ATwoPlayersSquareFieldController::GenerateField()
{
	int TerrainTypeIndex = 0;
	Cells = UCellTwoDimArray::New(Length, Width);
	
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
