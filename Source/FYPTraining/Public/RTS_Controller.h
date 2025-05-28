// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_Controller.generated.h"

class ASelectableObject;

UCLASS()
class FYPTRAINING_API ARTS_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void selectSpecificShip(ASelectableObject* selectedActor);
};