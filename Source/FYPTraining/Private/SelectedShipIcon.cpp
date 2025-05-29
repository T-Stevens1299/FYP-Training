// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedShipIcon.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RTS_Controller.h"
#include "Components/TextBlock.h"

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

void USelectedShipIcon::init(ASelectableObject* shipRef, FString iconId, float percent, int groupNum)
{
	changeHealth(percent);
	selectedShipRef = shipRef;

	//Sets ship icon
	ShipButton->WidgetStyle.Normal.SetResourceObject(shipIcons[iconId]);
	ShipButton->WidgetStyle.Hovered.SetResourceObject(shipIcons[iconId]);
	ShipButton->WidgetStyle.Pressed.SetResourceObject(shipIcons[iconId]);

	//sets group number if the group number is 0, no group is assigned and the number is hidden
	if (groupNum == 0) { GroupNumber->SetVisibility(ESlateVisibility::Hidden); }
	else
	{
		GroupNumber->SetVisibility(ESlateVisibility::Visible);
		GroupNumber->SetText(FText::FromString(FString::FromInt(groupNum)));
	}
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