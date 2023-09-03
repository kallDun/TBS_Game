#include "Field/Building/BuildingView.h"
#include "Field/Building/Building.h"
#include "Field/Controller/FieldController.h"
#include "Field/Event/TurnsOrderEventSystem.h"


void ABuildingView::Init(AFieldController* Field, ABuilding* BuildingReference, const FHexagonLocation HexagonLocation, const bool IsMainBuildingView)
{
	AGameActor::Init(Field);
	SetOwner(BuildingReference);
	BuildingRef = BuildingReference;
	Location = HexagonLocation;
	bIsMainBuildingView = IsMainBuildingView;
	UpdateWorldLocation();
	OnInit();
}


// -------------------------------------  Player pre/post move  -------------------------------------------

void ABuildingView::PrePlayerMoveTick()
{
	GetEventSystem()->BuildingViewPreMoveStarted.Broadcast(this);
	PrePlayerMoveTickBP();
	if (bAutomaticallyEndPrePlayerMove)
	{
		GetEventSystem()->BuildingViewPreMoveEnded.Broadcast(this);
	}
}

void ABuildingView::PostPlayerMoveTick()
{
	GetEventSystem()->BuildingViewPostMoveStarted.Broadcast(this);
	PostPlayerMoveTickBP();
	if (bAutomaticallyEndPostPlayerMove)
	{
		GetEventSystem()->BuildingViewPostMoveEnded.Broadcast(this);
	}
}


// -------------------------------------  Getters  --------------------------------------------------

UTurnsOrderEventSystem* ABuildingView::GetEventSystem() const
{
	return BuildingRef->GetFieldController()->GetTurnsOrderEventSystem();
}

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

