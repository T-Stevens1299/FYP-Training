// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableObject.h"
#include "Shipyard.generated.h"

class AFYPTrainingGameMode;

/**
 * 
 */
UCLASS()
class FYPTRAINING_API AShipyard : public ASelectableObject
{
	GENERATED_BODY()

public:
	//Functions
	void init(AFYPTrainingGameMode* gamemodeReference);

	void GenerateBaseIncome();

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnitTweakables")
	float baseIncomeRate;

	//References
	AFYPTrainingGameMode* gmRef;
	
	FTimerHandle incomeTimer;
};
