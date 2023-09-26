#pragma once
#include <Net/UnrealNetwork.h>

#include "CoreMinimal.h"
#include <UObject/Object.h>
#include "CellParamsOneDimArray.h"
#include "CellParamsTwoDimArray.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCellParamsTwoDimArrayIteratorDynamic, FHexagonLocation, Index, FCellParameters, Cell);
DECLARE_DELEGATE_TwoParams(FCellParamsTwoDimArrayIterator, FHexagonLocation Index, FCellParameters Cell);

UCLASS(BlueprintType)
class TBS_GAME_API UCellParamsTwoDimArray : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
	TArray<FCellParamsOneDimArray> Array = {};

public:
	UFUNCTION(BlueprintCallable)
	static UCellParamsTwoDimArray* New(const int LengthX, const int LengthY)
	{
		UCellParamsTwoDimArray* NewArray = NewObject<UCellParamsTwoDimArray>();
		for (int i = 0; i < LengthX; ++i)
		{
			NewArray->Array.Add(FCellParamsOneDimArray(LengthY));
		}
		return NewArray;
	}
	
	UFUNCTION(BlueprintCallable)
	FCellParameters GetCell(const FHexagonLocation Location)
	{
		return Array[Location.X].GetCell(Location.Y);
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCell(const FHexagonLocation Location, const FCellParameters Cell)
	{
		Array[Location.X].SetCell(Location.Y, Cell);
	}

	UFUNCTION(BlueprintCallable)
	FHexagonLocation GetLength() const
	{
		return FHexagonLocation(Array.Num(), Array[0].GetLength());
	}

	void ForEach(const FCellParamsTwoDimArrayIterator& IteratorFunction)
	{
		for (int i = 0; i < Array.Num(); ++i)
		{
			for (int j = 0; j < Array[i].GetLength(); ++j)
			{
				IteratorFunction.Execute(FHexagonLocation(i, j), Array[i].GetCell(j));
			}
		}
	}
	
	UFUNCTION(BlueprintCallable)
	void ForEachBP(const FCellParamsTwoDimArrayIteratorDynamic IteratorFunction)
	{
		ForEach(FCellParamsTwoDimArrayIterator::CreateLambda([IteratorFunction]
			(const FHexagonLocation Index, const FCellParameters& Cell)
		{
			IteratorFunction.Execute(Index, Cell);
		}));
	}

	 virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		DOREPLIFETIME( UCellParamsTwoDimArray, Array );
	}
};
