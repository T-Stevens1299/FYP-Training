// Fill out your copyright notice in the Description page of Project Settings.


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