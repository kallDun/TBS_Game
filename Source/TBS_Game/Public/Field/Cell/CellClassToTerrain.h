#pragma once
#include "TerrainType.h"
#include "CellClassToTerrain.generated.h"

enum class ETerrainType : uint8;

USTRUCT(BlueprintType)
struct FCellClassToTerrain
{
	GENERATED_BODY()

public:
	FCellClassToTerrain() : FCellClassToTerrain(nullptr, ETerrainType::Grass) {}	
	FCellClassToTerrain(const TSubclassOf<class ACell> CellClass, const ETerrainType TerrainType) : CellClass(CellClass), TerrainType(TerrainType) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACell> CellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETerrainType TerrainType;
};
