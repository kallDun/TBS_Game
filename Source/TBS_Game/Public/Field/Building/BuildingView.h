#pragma once
#include "CoreMinimal.h"
#include <GameFramework/Actor.h>
#include "BuildingViewState.h"
#include "Field/FieldActor.h"
#include "BuildingView.generated.h"

enum class EBuildingViewState : uint8;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Building))
class TBS_GAME_API ABuildingView : public AFieldActor
{
	GENERATED_BODY()

public:
	ABuildingView() {}
	void Init(AFieldController* Field, class ABuilding* BuildingReference, FHexagonLocation HexagonLocation, bool IsMainBuildingView);

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
	ABuilding* BuildingRef;

	UPROPERTY(BlueprintGetter=IsMainBuildingView, BlueprintSetter=SetMainBuildingView, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	bool bIsMainBuildingView;
	
	UPROPERTY(BlueprintGetter=GetCurrentHitPoints, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	float CurrentHitPoints;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State Properties")
	EBuildingViewState ViewState = EBuildingViewState::Initialize;

	
// Getters & Setters
public:
	UFUNCTION(BlueprintGetter)
	float GetCurrentHitPoints() const { return CurrentHitPoints; }

	UFUNCTION(BlueprintGetter)
	class UTurnsOrderEventSystem* GetEventSystem() const;

	UFUNCTION(BlueprintGetter)
	ABuilding* GetBuildingRef() const { return BuildingRef; }
	
	UFUNCTION(BlueprintGetter)
	bool CanRotate() const { return bCanRotate; }

	UFUNCTION(BlueprintGetter)
	bool IsMainBuildingView() const { return bIsMainBuildingView; }

	UFUNCTION(BlueprintSetter)
	void SetMainBuildingView(bool IsMainBuildingView);

	UFUNCTION(BlueprintCallable)
	void SetState(EBuildingViewState NewState);
	
	virtual bool IsPreviewState() const override;

	virtual bool IsNotBuiltState() const override;
	
// Methods
public:
	UFUNCTION(BlueprintCallable)
	void PrePlayerMoveTick();
	
	UFUNCTION(BlueprintCallable)
	void PostPlayerMoveTick();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RotateBuildingViewBP();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PrePlayerMoveTickBP();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PostPlayerMoveTickBP();
	
};
