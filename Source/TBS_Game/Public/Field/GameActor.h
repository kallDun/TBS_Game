#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "GameActor.generated.h"


UCLASS(Abstract, BlueprintType)
class TBS_GAME_API AGameActor : public AActor
{
	GENERATED_BODY()

public:
	AGameActor() {}

protected:
	void Init(class AFieldController* Field)
	{
		FieldController = Field;
	}

protected:
	UPROPERTY(BlueprintReadOnly, Getter = GetFieldController, meta = (AllowPrivateAccess = "true"))
	AFieldController* FieldController;

// Getters
public:
	UFUNCTION(BlueprintGetter)
	AFieldController* GetFieldController() const { return FieldController; }
};
