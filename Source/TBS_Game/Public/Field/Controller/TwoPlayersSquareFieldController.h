#pragma once
#include "CoreMinimal.h"
#include "FieldController.h"
#include "TwoPlayersSquareFieldController.generated.h"


UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Field))
class TBS_GAME_API ATwoPlayersSquareFieldController : public AFieldController
{
	GENERATED_BODY()

public:
	ATwoPlayersSquareFieldController() : AFieldController() {}

// Defaults properties
private:
	UPROPERTY(EditDefaultsOnly, Category = "Field Settings")
	int32 Width = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Field Settings")
	int32 Length = 60;

	// TODO: add percent for generation to terrain types / river properties / etc.

// Methods
protected:
	virtual void GenerateField() override;

protected:
	virtual FHexagonLocation GetPlayerCenterLocation(const int PlayerIndex) const override;
	
};
