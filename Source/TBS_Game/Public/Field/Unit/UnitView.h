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
	AUnitView()
	{
		bReplicates = true;
		AActor::SetReplicateMovement(true);
	}
	void Init(AFieldController* Field, class AUnit* UnitReference, FHexagonLocation HexagonLocation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInit();
	
	// Current state properties
protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated, ReplicatedUsing=OnRep_UnitRef)
	AUnit* UnitRef;
	
	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	float CurrentHitPoints;
	
	UPROPERTY(BlueprintGetter=GetCurrentDefence, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	float CurrentDefence;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties", Replicated)
	EUnitViewState State = EUnitViewState::Initialize;

	
// Getters & Setters
public:
	UFUNCTION(BlueprintGetter)
	float GetCurrentHitPoints() const { return CurrentHitPoints; }
	
	UFUNCTION(BlueprintGetter)
	float GetCurrentDefence() const { return CurrentDefence; }

	UFUNCTION(BlueprintGetter)
	AUnit* GetUnitRef() const { return UnitRef; }

	UFUNCTION(BlueprintCallable)
	void SetState(EUnitViewState NewState);

	UFUNCTION(BlueprintCallable)
	void StartAssembling();
	
	virtual bool IsPreviewState() const override;

	virtual bool IsNotBuiltState() const override;
	
// Methods
public:
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_UnitRef();
};
