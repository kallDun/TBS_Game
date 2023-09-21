#include "Field/Unit/UnitView.h"
#include "Field/Controller/FieldController.h"
#include "Field/Unit/Unit.h"


void AUnitView::Init(AFieldController* Field, AUnit* UnitReference, const FHexagonLocation HexagonLocation)
{
	AGameActor::Init(Field);
	Tags.Add("PCG_OBSTACLE");
	SetOwner(UnitReference);
	UnitRef = UnitReference;
	Location = HexagonLocation;
	UpdateWorldLocation();
	OnInit();
}


// -------------------------------------  Player pre/post move  -------------------------------------------

void AUnitView::PrePlayerMoveTick()
{
	//GetEventSystem()->UnitViewPreMoveStarted.Broadcast(this);
	PrePlayerMoveTickBP();
	if (bAutomaticallyEndPrePlayerMove)
	{
		//GetEventSystem()->UnitViewPreMoveEnded.Broadcast(this);
	}
}

void AUnitView::PostPlayerMoveTick()
{
	//GetEventSystem()->UnitViewPostMoveStarted.Broadcast(this);
	PostPlayerMoveTickBP();
	if (bAutomaticallyEndPostPlayerMove)
	{
		//GetEventSystem()->UnitViewPostMoveEnded.Broadcast(this);
	}
}


// -------------------------------------  Getters  --------------------------------------------------

bool AUnitView::IsPreviewState() const
{
	return ViewState == EUnitViewState::Preview;
}

bool AUnitView::IsNotBuiltState() const
{
	return ViewState == EUnitViewState::Initialize;
}


// -------------------------------------  Setters  --------------------------------------------------

void AUnitView::SetState(const EUnitViewState NewState)
{
	ViewState = NewState;
}

