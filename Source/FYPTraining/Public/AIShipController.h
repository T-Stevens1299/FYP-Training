// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIShipController.generated.h"

class ASelectableObject;


UCLASS()
class FYPTRAINING_API AAIShipController : public AAIController
{
	GENERATED_BODY()

	virtual void BeginPlay();

public:
	ASelectableObject* controlledShip;

private:
	FTimerHandle behaviourTreeTick;

	void checkOrderCode();
	
};
