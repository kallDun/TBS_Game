#pragma once
#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "CellOneDimArray.h"
#include "Field/HexagonLocation.h"
#include "CellTwoDimArray.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCellTwoDimArrayIteratorDynamic, FHexagonLocation, Index, ACell*, Cell);
DECLARE_DELEGATE_TwoParams(FCellTwoDimArrayIterator, FHexagonLocation Index, ACell* Cell);

UCLASS(BlueprintType)
class TBS_GAME_API UCellTwoDimArray : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
	TArray<UCellOneDimArray*> Array = {};

public:
	UFUNCTION(BlueprintCallable)
	static UCellTwoDimArray* New(const int LengthX, const int LengthY)
	{
		UCellTwoDimArray* NewArray = NewObject<UCellTwoDimArray>();
		for (int i = 0; i < LengthX; ++i)
		{
			NewArray->Array.Add(UCellOneDimArray::New(LengthY));
		}
		return NewArray;
	}
	
	UFUNCTION(BlueprintCallable)
	ACell* GetCell(const FHexagonLocation Location)
	{
		return Array[Location.X]->GetCell(Location.Y);
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCell(const FHexagonLocation Location, ACell* Cell)
	{
		Array[Location.X]->SetCell(Location.Y, Cell);
	}

	UFUNCTION(BlueprintCallable)
	FHexagonLocation GetLength() const
	{
		return FHexagonLocation(Array.Num(), Array[0]->GetLength());
	}

	void ForEach(const FCellTwoDimArrayIterator& IteratorFunction)
	{
		for (int i = 0; i < Array.Num(); ++i)
		{
			for (int j = 0; j < Array[i]->GetLength(); ++j)
			{
				IteratorFunction.Execute(FHexagonLocation(i, j), Array[i]->GetCell(j));
			}
		}
	}
	
	UFUNCTION(BlueprintCallable)
	void ForEachBP(const FCellTwoDimArrayIteratorDynamic IteratorFunction)
	{
		ForEach(FCellTwoDimArrayIterator::CreateLambda([IteratorFunction]
			(const FHexagonLocation Index, ACell* Cell)
		{
			IteratorFunction.Execute(Index, Cell);
		}));
	}

	__override virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		DOREPLIFETIME( UCellTwoDimArray, Array );
	}
};
