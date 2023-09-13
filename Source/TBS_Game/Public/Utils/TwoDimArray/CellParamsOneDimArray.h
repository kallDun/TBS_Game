#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "Field/Anchor/CellParameters.h"
#include "CellParamsOneDimArray.generated.h"


USTRUCT(BlueprintType)
struct FCellParamsOneDimArray
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<FCellParameters> Array = {};
	
public:
	FCellParamsOneDimArray() = default;
	
	explicit FCellParamsOneDimArray(const int Length)
	{
		Array.SetNum(Length);
	}
	
	FCellParameters GetCell(const int Index)
	{
		return Array[Index];
	}
	
	void SetCell(const int Index, FCellParameters Cell)
	{
		Array[Index] = Cell;
	}

	int GetLength() const
	{
		return Array.Num();
	}
};
