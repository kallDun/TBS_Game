

#include "Field/Unit/UnitView.h"
#include "Field/Controller/FieldController.h"
#include "Field/Event/TurnsOrderEventSystem.h"
#include "Field/Unit/Unit.h"


void AUnitView::Init(AFieldController* Field, AUnit* UnitReference, const FHexagonLocation HexagonLocation)
{
	AGameActor::Init(Field);
	SetOwner(UnitReference);
	UnitRef = UnitReference;
	Location = HexagonLocation;
	UpdateWorldLocation();
	OnInit();
}


// -------------------------------------  Player pre/post move  -------------------------------------------

void AUnitView::PrePlayerMoveTick()
{
	GetEventSystem()->UnitViewPreMoveStarted.Broadcast(this);
	PrePlayerMoveTickBP();
	if (bAutomaticallyEndPrePlayerMove)
	{
		GetEventSystem()->UnitViewPreMoveEnded.Broadcast(this);
	}
}

void AUnitView::PostPlayerMoveTick()
{
	GetEventSystem()->UnitViewPostMoveStarted.Broadcast(this);
	PostPlayerMoveTickBP();
	if (bAutomaticallyEndPostPlayerMove)
	{
		GetEventSystem()->UnitViewPostMoveEnded.Broadcast(this);
	}
}


// -------------------------------------  Getters  --------------------------------------------------

UTurnsOrderEventSystem* AUnitView::GetEventSystem() const
{
	return UnitRef->GetFieldController()->GetTurnsOrderEventSystem();
}

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

