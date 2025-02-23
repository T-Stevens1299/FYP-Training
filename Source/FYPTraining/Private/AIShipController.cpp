// Fill out your copyright notice in the Description page of Project Settings.


#include "AIShipController.h"
#include "SelectableObject.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIShipController::BeginPlay()
{
	Super::BeginPlay();

}

void AAIShipController::checkOrderCode()
{
	switch (controlledShip->orderCode)
	{
	case 0:
		break;

	case 1:
		controlledShip->moveObject(controlledShip->retreatPointRef->GetActorLocation(), 500.0f);
		UE_LOG(LogTemp, Warning, TEXT("Order 1 Recieved"));
		break;

	case 2:
		if (controlledShip->CurrentTarget == NULL)
		{
			controlledShip->moveObject(controlledShip->attackPointRef->GetActorLocation(), controlledShip->WeaponsRange);
			UE_LOG(LogTemp, Warning, TEXT("Order 2 Recieved"));
		}
		break;

	default:
		break;
	}

}