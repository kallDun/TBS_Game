#include "Field/Hero/Hero.h"


void AHero::Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner)
{
	PlayerControllerRef = PlayerControllerOwner;
	AGameActor::Init(Field);
}

bool AHero::TryToSpawn(TArray<FHexagonLocation> HexagonLocations)
{
	return true;
}

bool AHero::CanSpawnOnLocation(FHexagonLocation HexagonLocation)
{
	return true;
}

bool AHero::CanUnlockNewAbility(AGamePlayerController* PlayerController)
{
	return true;
}

