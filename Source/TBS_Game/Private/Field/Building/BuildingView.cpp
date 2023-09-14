#include "Field/Building/BuildingView.h"
#include "Net/UnrealNetwork.h"
#include "Field/Building/Building.h"
#include "Field/Controller/FieldController.h"
#include "Field/Event/TurnsOrderEventSystem.h"


void ABuildingView::Init(AFieldController* Field, ABuilding* BuildingReference, const FHexagonLocation HexagonLocation, const bool IsMainBuildingView)
{
	AGameActor::Init(Field);
	Tags.Add("PCG_OBSTACLE");
	SetOwner(BuildingReference);
	BuildingRef = BuildingReference;
	Location = HexagonLocation;
	bIsMainBuildingView = IsMainBuildingView;
	UpdateWorldLocation();
	OnInit();
}

void ABuildingView::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( ABuildingView, bCanRotate );
	DOREPLIFETIME( ABuildingView, BuildingRef );
	DOREPLIFETIME( ABuildingView, bIsMainBuildingView );
	DOREPLIFETIME( ABuildingView, CurrentHitPoints );
	DOREPLIFETIME( ABuildingView, ViewState );
}


// -------------------------------------  Player pre/post move  -------------------------------------------

void ABuildingView::StartPreMoveTick_Implementation()
{
	BuildingRef->EndBuildingViewPreMove(this);
}

void ABuildingView::StartPostMoveTick_Implementation()
{
	BuildingRef->EndBuildingViewPostMove(this);
}


// -------------------------------------  Getters  --------------------------------------------------

bool ABuildingView::IsPreviewState() const
{
	return ViewState == EBuildingViewState::Preview;
}

bool ABuildingView::IsNotBuiltState() const
{
	return ViewState == EBuildingViewState::Initialize;
}


// -------------------------------------  Setters  --------------------------------------------------

void ABuildingView::SetMainBuildingView(const bool IsMainBuildingView)
{
	bIsMainBuildingView = IsMainBuildingView;
}

void ABuildingView::SetState(const EBuildingViewState NewState)
{
	ViewState = NewState;
}

