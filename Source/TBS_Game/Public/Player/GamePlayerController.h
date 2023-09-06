#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "PlayerTurnType.h"
#include "Field/GameActor.h"
#include "Field/HexagonLocation.h"
#include "GameFramework/Character.h"
#include "GamePlayerController.generated.h"

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

// State
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	EPlayerTurnType PlayerTurnType = EPlayerTurnType::Waiting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	FHexagonLocation CenterLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int EconomicPoints = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int ReligiousFollowers = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int PlayerNumber;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	int MovesLeft = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	TArray<ABuilding*> BuildingPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State", Replicated)
	TArray<ABuilding*> Buildings = {};

// --------------------------------- not implemented in BPs ----------------------------------
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AUnit*> UnitPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AHero*> HeroPrefabs = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	TArray<AUnit*> Units = {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "State")
	AUnit* Hero;

// -------------------------------------------------------------------------------------------

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
protected:
	UFUNCTION()
	void Init(int Index);
	
	UFUNCTION(Server, Reliable)
	void InitState(int PlayerNum, FHexagonLocation CenterHexLocation);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SubscribeEventHandlersBP(class UTurnsOrderEventSystem* TurnsOrderEventSystem);
	
	UFUNCTION()
	void InitBuildingPrefabs();

	UFUNCTION()
	ABuilding* InitBuildingPrefab(TSubclassOf<ABuilding> BuildingClass);

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
