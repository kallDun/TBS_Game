#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TBS_GameModeBase.generated.h"

class AFieldController;
class AGamePlayerController;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (GameMode))
class TBS_GAME_API ATBS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
};
