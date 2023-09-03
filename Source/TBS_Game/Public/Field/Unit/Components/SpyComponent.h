// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBehaviourComponent.h"
#include "Components/ActorComponent.h"
#include "SpyComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS_GAME_API USpyComponent : public UUnitBehaviourComponent
{
	GENERATED_BODY()

public:
	USpyComponent() {}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Spy")
	float Vision;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetVision() const { return Vision; }
};
