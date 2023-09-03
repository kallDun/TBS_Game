#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "CellOneDimArray.generated.h"

class ACell;

UCLASS(BlueprintType)
class TBS_GAME_API UCellOneDimArray : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<ACell*> Array = {};

public:
	UFUNCTION(BlueprintCallable)
	static UCellOneDimArray* New(const int Length)
	{
		UCellOneDimArray* NewArray = NewObject<UCellOneDimArray>();
		NewArray->Array.SetNum(Length);
		return NewArray;
	}
	
	UFUNCTION(BlueprintCallable)
	ACell* GetCell(const int Index)
	{
		return Array[Index];
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCell(const int Index, ACell* Cell)
	{
		Array[Index] = Cell;
	}
	
	UFUNCTION(BlueprintCallable)
	int GetLength() const
	{
		return Array.Num();
	}
};
