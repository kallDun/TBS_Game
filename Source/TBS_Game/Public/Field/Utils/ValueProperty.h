#pragma once
#include "ValueProperty.generated.h"

USTRUCT(BlueprintType)
struct FValueProperty
{
	GENERATED_BODY()

public:
	FValueProperty() = default;
	FValueProperty(const FName& InPropertyName, const float& InPropertyValue)
		: Name(InPropertyName)
		, Value(InPropertyValue) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	void SetValue(const float& InValue)
	{
		Value = InValue;
	}

	FValueProperty Clone() const
	{
		return FValueProperty(Name, Value);
	}
};
