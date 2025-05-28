// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedShipsTab.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "SelectedShipIcon.h"
#include "Components/WrapBox.h"
#include "Components/PanelWidget.h"

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
		USelectedShipIcon* newIcon = CreateWidget<USelectedShipIcon>(gmRef->PC, iconUiRef);
		newIcon->init("", 1);
		shipIcons.Add(newIcon);
		IconContainer->AddChild(newIcon);
	}
}

void USelectedShipsTab::updateHealthBar(int iconArrayIndex, float newHealthPercent)
{
	shipIcons[iconArrayIndex]->changeHealth(newHealthPercent);
}