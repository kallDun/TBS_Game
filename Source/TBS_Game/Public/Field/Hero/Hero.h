// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeroState.h"
#include "Field/FieldActor.h"
#include "Hero.generated.h"

class ACell;
class ABuilding;
class AGamePlayerController;
enum class EHeroPillarType : uint8;
enum class EHeroState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Hero))
class TBS_GAME_API AHero : public AFieldActor
{
	GENERATED_BODY()

public:
	AHero() {}
	void Init(AFieldController* Field, AGamePlayerController* PlayerControllerOwner);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Main")
	FName HeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Main")
	FName HeroDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Main")
	EHeroPillarType HeroType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Main")
	float MaxHitPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Main")
	float MaxDefence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "0.5", ClampMax = "5"), Category = "Hero Main")
	float NumSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "3"), Category = "Hero Main")
	int Level = 1;

	// info properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Info")
	FName MainRequirementsToSpawnInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Hero Info")
	FName ImproveLevelFromLocationInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Hero Info")
	UTexture2D* HeroIconMedium;

private:
	UPROPERTY(BlueprintGetter=GetPlayerControllerRef, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	AGamePlayerController* PlayerControllerRef = nullptr;

	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	float CurrentHitPoints;

	UPROPERTY(BlueprintGetter=GetCurrentDefence, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	float CurrentDefence;

	UPROPERTY(BlueprintGetter=GetHeroState, meta = (AllowPrivateAccess = "true"), Category = "Properties")
	EHeroState HeroState = EHeroState::Blocked;

// Getters
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGamePlayerController* GetPlayerControllerRef() const { return PlayerControllerRef; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHitPoints() const { return MaxHitPoints; };
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EHeroState GetHeroState() const { return HeroState; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxDefence() const { return MaxDefence; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetStepsNumber() const { return NumSteps; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentHitPoints() const { return CurrentHitPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentDefence() const { return CurrentDefence; }

public:
	UFUNCTION(BlueprintCallable)
	bool TryToSpawn(TArray<FHexagonLocation> HexagonLocations);
	
public:	
	UFUNCTION(BlueprintCallable)
	bool CanSpawnOnLocation(FHexagonLocation HexagonLocation);
	
	UFUNCTION(BlueprintCallable)
	bool CanUnlockNewAbility(AGamePlayerController* PlayerController);
};
