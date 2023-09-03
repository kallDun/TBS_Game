#include "Field/Building/UpgradeBuildingComponent.h"
#include "Field/Building/BuildingViewState.h"
#include "Field/ReturnState/BuildUpgradeReturnState.h"


// ------------------ Upgrade Logic ------------------

EBuildUpgradeReturnState UUpgradeBuildingComponent::TryToUpgrade()
{
	if (UpgradingState != EUpgradingState::Initialized)
	{
		return EBuildUpgradeReturnState::IncorrectState;
	}
	if (!CanUpgrade())
	{
		return EBuildUpgradeReturnState::RequirementsNotMet;	
	}

	ABuilding* Building = GetBuildingRef();
	const int ImproveLevel = Building->GetImproveLevelFromLocation(Building->InitBuildingLocation);
	ChangePropertiesAccordingToImproveLevel(ImproveLevel);	
	UpgradingState = EUpgradingState::Building;
	MovesToBuildLeft = MovesToBuild - 1;
	MovesToAssembleLeft = MovesToAssemble;
	Building->ChangeBuildingViewsState(EBuildingViewState::Upgrading);
	if (MovesToBuildLeft == 0)
	{
		TryToContinueUpgrading();
	}
	return EBuildUpgradeReturnState::Succeeded;
}

EBuildUpgradeReturnState UUpgradeBuildingComponent::TryToContinueUpgrading()
{
	// TODO: check cell and return false state or change properties
	
	if (UpgradingState == EUpgradingState::Building)
	{
		MovesToBuildLeft--;
		if (MovesToBuildLeft <= 0)
		{
			UpgradingState = EUpgradingState::Assembling;
			if (MovesToAssembleLeft <= 0)
			{
				AssembleMoveTick();
			}
		}
		return EBuildUpgradeReturnState::Succeeded;
	}
	return EBuildUpgradeReturnState::IncorrectState;
}


// ------------------ Player Move Tick ------------------

void UUpgradeBuildingComponent::PrePlayerMoveTick_Implementation()
{
}

void UUpgradeBuildingComponent::PostPlayerMoveTick_Implementation()
{
	
}

void UUpgradeBuildingComponent::AssembleMoveTick()
{
	if (UpgradingState == EUpgradingState::Assembling)
	{
		MovesToAssembleLeft--;
		if (MovesToAssembleLeft == 0)
		{
			UpgradingState = EUpgradingState::Ready;
			GetBuildingRef()->ChangeBuildingViewsState(EBuildingViewState::Ready);
			OnUpgradeAction();
		}
	}
}


// ------------------ Getters & Setters ------------------

float UUpgradeBuildingComponent::GetPropertyValue(FName PropertyName, float ValueByDefault) const
{
	for (FBuildingProperty Property : Properties)
	{
		if (Property.Name == PropertyName)
		{
			return Property.Value;
		}
	}
	return ValueByDefault;
}

bool UUpgradeBuildingComponent::SetPropertyValue(const FName PropertyName, const float Value) const
{
	for (FBuildingProperty Property : Properties)
	{
		if (Property.Name == PropertyName)
		{
			Property.SetValue(Value);
			return true;
		}
	}
	return false;
}
