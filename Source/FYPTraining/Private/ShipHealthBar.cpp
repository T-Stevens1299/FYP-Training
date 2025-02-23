// Written by Thomas Stevens, all rights reserved

#include "ShipHealthBar.h"
#include "Components/ProgressBar.h"

UShipHealthBar::UShipHealthBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UShipHealthBar::updateHealthBar(float passedPercentage)
{
	HealthBar->SetPercent(passedPercentage);
}