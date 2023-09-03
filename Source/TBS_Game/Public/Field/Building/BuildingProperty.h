#pragma once
#include "BuildingProperty.generated.h"

USTRUCT(BlueprintType)
struct FBuildingProperty
{
	GENERATED_BODY()

public:
	FBuildingProperty() = default;
	FBuildingProperty(const FName& InPropertyName, const float& InPropertyValue)
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

	FBuildingProperty Clone() const
	{
		return FBuildingProperty(Name, Value);
	}
};
