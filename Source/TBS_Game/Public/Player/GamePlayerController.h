#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PlayerInitializationState.h"
#include "PlayerTurnType.h"
#include "Field/GameActor.h"
#include "Field/HexagonLocation.h"
#include "Field/ReturnState/UnitUpgradeReturnState.h"
#include "GameFramework/Character.h"
#include "GamePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerInitialized);

class AFieldActor;
class ATBS_GameModeBase;
class AUnit;
class AHero;
class ABuilding;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = (Player))
class TBS_GAME_API AGamePlayerController : public ACharacter
{
	GENERATED_BODY()

public:
	AGamePlayerController();

// Components
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

// Default values
private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Input")
	UInputMappingContext* PlayerInputMappingContext;
	
// Properties
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	EPlayerInitializationState InitializationState = EPlayerInitializationState::NotInitialized;

// State
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	EPlayerTurnType PlayerTurnType = EPlayerTurnType::Waiting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated, Category = "State")
	FHexagonLocation CenterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated, Category = "State")
	int EconomicPoints = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated, Category = "State")
	int ReligiousFollowers = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated, Category = "State")
	int PlayerNumber;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated, Category = "State")
	int MovesLeft = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<ABuilding*> BuildingPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AUnit*> UnitPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AHero*> HeroPrefabs = {};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<ABuilding*> Buildings = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AUnit*> Units = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	AUnit* Hero;

	UPROPERTY(BlueprintAssignable)
	FPlayerInitialized PlayerInitializedEvent;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AFieldController* FieldController;

// Getters
public:	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

	
// Base overrides
protected:
	virtual void BeginPlay() override;	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Initialization
public:
	UFUNCTION()
	void Init(int PlayerNum, FHexagonLocation CenterHexLocation);
	
protected:	
	UFUNCTION(BlueprintImplementableEvent)
	void InitEventHandlersBP(class UTurnsOrderEventSystem* TurnsOrderEventSystem);
	
	UFUNCTION(Server, Reliable)
	void InitPlayerNumberAndCenterLocation(int PlayerNum, FHexagonLocation HexagonLocation);
	
	UFUNCTION(Server, Reliable)
	void SetInitializationStateLoaded();
	
	UFUNCTION(NetMulticast, Reliable)
	void InitBuildingPrefabs();

	UFUNCTION(NetMulticast, Reliable)
	void InitUnitPrefabs();

	UFUNCTION(NetMulticast, Reliable)
	void InitHeroPrefabs();

	UFUNCTION()
	ABuilding* InitBuildingPrefab(TSubclassOf<ABuilding> BuildingClass);

	UFUNCTION()
	AUnit* InitUnitPrefab(TSubclassOf<AUnit> UnitClass);

	UFUNCTION()
	AHero* InitHeroPrefab(TSubclassOf<AHero> HeroClass);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InitInputComponent();

	
// Methods
public:	
	UFUNCTION(NetMulticast, Reliable)
	void StartTurn();

	UFUNCTION()
	bool CanUseMove() const;
	
	UFUNCTION()
	bool TryToUseMove();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckIfPlayerMove() const;

protected:	
	UFUNCTION()
	void StartBuildingsAssembling();

	UFUNCTION()
	void StartPlayersMove();

	UFUNCTION()
	void StartBuildingsPreMove();
	
	UFUNCTION()
	void StartBuildingsPostMove();
	
	UFUNCTION()
	void EndTurn();

// Actions
	UFUNCTION(BlueprintCallable)
	EBuildUpgradeReturnState ConstructBuilding(ABuilding* Building);

	UFUNCTION(BlueprintCallable)
	EUnitUpgradeReturnState ConstructUnit(AUnit* Unit);

// Event handlers
private:
	UPROPERTY()
	int BuildingMoveCalledCount = 0;

	UFUNCTION()
	void DoNextBuildingPreMove();
	
	UFUNCTION()
	void DoNextBuildingPostMove();
	
	UFUNCTION()
	void BuildingPreMoveEndedEventHandler(ABuilding* Building);

	UFUNCTION()
	void BuildingPostMoveEndedEventHandler(ABuilding* Building);
	
// Auxiliary
protected:
	UFUNCTION(BlueprintCallable)
	void SetPlayerTurnType(EPlayerTurnType NewPlayerTurnType);
	
	UFUNCTION(BlueprintCallable)
	AActor* GetActorUnderCursor() const;

	UFUNCTION(BlueprintCallable)
	void SetActorLocationAndRotation(FHexagonLocation HexagonLocation, const FRotator& WorldRotation);

	UFUNCTION(Server, Reliable)
	void SetActorLocationAndRotationServer(FVector WorldLocation, FRotator WorldRotation);
};
