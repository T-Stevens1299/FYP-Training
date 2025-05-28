// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedShipIcon.h"
#include "Components/ProgressBar.h"

USelectedShipIcon::USelectedShipIcon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USelectedShipIcon::NativeConstruct()
{

}

void USelectedShipIcon::init(FString iconId, float percent)
{
	changeHealth(percent);
	//change icon
}

void USelectedShipIcon::changeHealth(float percent)
{
	shipHealth->SetPercent(percent);
}