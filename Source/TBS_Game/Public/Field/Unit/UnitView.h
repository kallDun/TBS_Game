#pragma once

#include "CoreMinimal.h"
#include "UnitViewState.h"
#include "Field/FieldActor.h"
#include "GameFramework/Actor.h"
#include "UnitView.generated.h"

enum class EUnitViewState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Unit))
class TBS_GAME_API AUnitView : public AFieldActor
{
	GENERATED_BODY()

public:
	AUnitView() {}
	void Init(AFieldController* Field, class AUnit* UnitReference, FHexagonLocation HexagonLocation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInit();
	
	// Current state properties
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	bool bAutomaticallyEndPrePlayerMove = true;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	bool bAutomaticallyEndPostPlayerMove = true;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	bool bCanRotate = true;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	AUnit* UnitRef;
	
	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	float CurrentHitPoints;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	EUnitViewState ViewState = EUnitViewState::Initialize;

	
// Getters & Setters
public:
	UFUNCTION(BlueprintGetter)
	float GetCurrentHitPoints() const { return CurrentHitPoints; }

	UFUNCTION(BlueprintGetter)
	AUnit* GetUnitRef() const { return UnitRef; }
	
	UFUNCTION(BlueprintGetter)
	bool CanRotate() const { return bCanRotate; }

	UFUNCTION(BlueprintCallable)
	void SetState(EUnitViewState NewState);
	
	virtual bool IsPreviewState() const override;

	virtual bool IsNotBuiltState() const override;
	
// Methods
public:
	UFUNCTION(BlueprintCallable)
	void PrePlayerMoveTick();
	
	UFUNCTION(BlueprintCallable)
	void PostPlayerMoveTick();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PrePlayerMoveTickBP();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PostPlayerMoveTickBP();
	
};
