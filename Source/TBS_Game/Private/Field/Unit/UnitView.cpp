#include "Field/Unit/UnitView.h"
#include <Net/UnrealNetwork.h>
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

void AUnitView::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AUnitView, UnitRef );
	DOREPLIFETIME( AUnitView, CurrentHitPoints );
	DOREPLIFETIME( AUnitView, CurrentDefence );
	DOREPLIFETIME( AUnitView, State );
	DOREPLIFETIME( AUnitView, MovesToAssembleLeft );
}

// -------------------------------------  Getters  --------------------------------------------------

bool AUnitView::IsPreviewState() const
{
	return State == EUnitViewState::Preview;
}

bool AUnitView::IsNotBuiltState() const
{
	return State == EUnitViewState::Initialize;
}

// -------------------------------------  Setters  --------------------------------------------------

void AUnitView::SetState(const EUnitViewState NewState)
{
	State = NewState;
}

// -------------------------------------  Methods  --------------------------------------------------

void AUnitView::StartAssembling()
{
	MovesToAssembleLeft = GetUnitRef()->MovesToAssemble;
	if (MovesToAssembleLeft > 0)
	{
		State = EUnitViewState::Assembling;
	}
	else
	{
		State = EUnitViewState::Idle;
	}
}

void AUnitView::AssembleMoveTick()
{
	if (MovesToAssembleLeft > 0)
	{
		MovesToAssembleLeft--;
		if (MovesToAssembleLeft == 0)
		{
			State = EUnitViewState::Idle;
		}
	}
}

void AUnitView::StartMoveTick_Implementation()
{
	GetUnitRef()->EndUnitViewMoveTick(this);
}

