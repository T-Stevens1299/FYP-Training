// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedShipsTab.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "SelectableObject.h"
#include "SelectedShipIcon.h"
#include "Components/WrapBox.h"
#include "Components/PanelWidget.h"
#include "SelectableObject.h"

USelectedShipsTab::USelectedShipsTab(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USelectedShipsTab::NativeConstruct()
{

}

void USelectedShipsTab::init(AFYPTrainingGameMode* gameModeRef)
{
	gmRef = gameModeRef;
}

void USelectedShipsTab::populateIcons()
{
	//Deletes previous icon references
	IconContainer->ClearChildren();
	shipIcons.Empty();

	//Repopulates
	for (int i = 0; i < gmRef->SelectedShips.Num(); i++)
	{
		if (gmRef->SelectedShips[i] != nullptr)
		{
			if (gmRef->SelectedShips[i])
			{
				USelectedShipIcon* newIcon = CreateWidget<USelectedShipIcon>(gmRef->PC, iconUiRef);
				float percent = gmRef->SelectedShips[i]->currentUnitHealth / gmRef->SelectedShips[i]->totalUnitHealth;

				newIcon->init(gmRef->SelectedShips[i], gmRef->SelectedShips[i]->shipTypeName, percent, gmRef->SelectedShips[i]->groupNumber);
				shipIcons.Add(newIcon);

				IconContainer->AddChild(newIcon);
			}
		}
	}
}

void USelectedShipsTab::updateHealthBar(int iconArrayIndex, float newHealthPercent)
{
	if (shipIcons.IsValidIndex(iconArrayIndex))
	{
		shipIcons[iconArrayIndex]->changeHealth(newHealthPercent);
	}
}