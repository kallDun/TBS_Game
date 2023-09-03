#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "Field/Anchor/CellParameters.h"
#include "CellParamsOneDimArray.generated.h"


UCLASS(BlueprintType)
class TBS_GAME_API UCellParamsOneDimArray : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<FCellParameters> Array = {};
	
public:
	UFUNCTION(BlueprintCallable)
	static UCellParamsOneDimArray* New(const int Length)
	{
		UCellParamsOneDimArray* NewArray = NewObject<UCellParamsOneDimArray>();
		NewArray->Array.SetNum(Length);
		return NewArray;
	}
	
	UFUNCTION(BlueprintCallable)
	FCellParameters GetCell(const int Index)
	{
		return Array[Index];
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCell(const int Index, FCellParameters Cell)
	{
		Array[Index] = Cell;
	}

	UFUNCTION(BlueprintCallable)
	int GetLength() const
	{
		return Array.Num();
	}
};
