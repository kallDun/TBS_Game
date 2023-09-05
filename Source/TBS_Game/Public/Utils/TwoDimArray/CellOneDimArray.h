#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "CellOneDimArray.generated.h"

class ACell;

USTRUCT(BlueprintType)
struct FCellOneDimArray
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<ACell*> Array = {};

public:
	FCellOneDimArray() = default;

	explicit FCellOneDimArray(const int Length)
	{
		Array.SetNum(Length);
	}
	
	ACell* GetCell(const int Index)
	{
		return Array[Index];
	}
	
	void SetCell(const int Index, ACell* Cell)
	{
		Array[Index] = Cell;
	}
	
	int GetLength() const
	{
		return Array.Num();
	}
};
