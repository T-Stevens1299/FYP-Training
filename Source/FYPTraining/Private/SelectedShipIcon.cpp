// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedShipIcon.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RTS_Controller.h"

USelectedShipIcon::USelectedShipIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void USelectedShipIcon::NativeConstruct()
{
	if (ShipButton)
	{
		ShipButton->OnClicked.AddDynamic(this, &USelectedShipIcon::selectShipInIcon);
	}
}

void USelectedShipIcon::init(ASelectableObject* shipRef, FString iconId, float percent)
{
	changeHealth(percent);
	selectedShipRef = shipRef;

	ShipButton->WidgetStyle.Normal.SetResourceObject(shipIcons[iconId]);
	ShipButton->WidgetStyle.Hovered.SetResourceObject(shipIcons[iconId]);
	ShipButton->WidgetStyle.Pressed.SetResourceObject(shipIcons[iconId]);
}

void USelectedShipIcon::changeHealth(float percent)
{
	shipHealth->SetPercent(percent);
}

void USelectedShipIcon::selectShipInIcon()
{
	ARTS_Controller* PC = Cast<ARTS_Controller>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC) { PC->selectSpecificShip(selectedShipRef); }
}