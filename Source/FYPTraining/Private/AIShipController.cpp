// Fill out your copyright notice in the Description page of Project Settings.


#include "AIShipController.h"
#include "SelectableObject.h"

void AAIShipController::BeginPlay()
{
	Super::BeginPlay();

	ASelectableObject* AIShip = Cast<ASelectableObject>(GetPawn());
	if (AIShip && AIShip->BT_Asset)
	{
		if (!(AIShip->playerControlled))
		{
			RunBehaviorTree(AIShip->BT_Asset);
		}
	}
}