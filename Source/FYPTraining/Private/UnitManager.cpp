// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"
#include "Shipyard.h"
#include "Engine/DataTable.h"
#include "ShipyardWidget.h"
#include "FYPTraining/FYPTrainingGameMode.h"

// Sets default values
AUnitManager::AUnitManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUnitManager::Init(AFYPTrainingGameMode* gmRef)
{
	gamemodeRef = gmRef;
	currentTechLevel = 1;
	addNewUnitOptions();
}

void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	startUnitBuildingOperation();
}

void AUnitManager::setShipyardPtr(AShipyard* shipyardRef)
{
	shipYardRef = shipyardRef;
	UE_LOG(LogTemp, Warning, TEXT("Shipyard Ref Set"));
}

void AUnitManager::upgradeTechLevel(FConstructionData* currentRowRef)
{ 
	currentRow = currentRowRef;
	UE_LOG(LogTemp, Warning, TEXT("TriggerAIUpgrade"));
	if ((gamemodeRef->currentAIMoney - currentRow->requiredFunds) >= 0)
	{
		gamemodeRef->subtractCost(false, currentRow->requiredFunds);
	}
	else
	{
		gamemodeRef->currentAIMoney = 0;
	}

	currentTechLevel++; 
	gamemodeRef->aiTechLevel = currentTechLevel; 
	addNewUnitOptions();
}

void AUnitManager::addNewUnitOptions()
{
	//adds the units that can be built to the available units to build array. Finds the unit weight and adds it to the weight array
	switch (currentTechLevel)
	{
	case 1:
		availableUnitsToBuild.Add("Corvette");
		unitWeights.Add(findUnitWeightValue("Corvette"));
		break;

	case 2:
		availableUnitsToBuild.Add("Frigate");
		unitWeights.Add(findUnitWeightValue("Frigate"));
		availableUnitsToBuild.Add("Destroyer");
		unitWeights.Add(findUnitWeightValue("Destroyer"));
		break;

	case 3:
		availableUnitsToBuild.Add("Cruiser");
		unitWeights.Add(findUnitWeightValue("Cruiser"));
		break;

	case 4:
		availableUnitsToBuild.Add("Battleship");
		unitWeights.Add(findUnitWeightValue("Battleship"));
		availableUnitsToBuild.Add("AircraftCarrier");
		unitWeights.Add(findUnitWeightValue("AircraftCarrier"));
		break;

	default:
		break;
	}
}

void AUnitManager::startUnitBuildingOperation()
{
	int unitToBuild = determineUnitToBuild();
	currentRow = findCurrentRow(availableUnitsToBuild[unitToBuild]);
	float shipCost = currentRow->requiredFunds;
	float buildTime = currentRow->constructionTime;
	int popValue = currentRow->populationValue;

	if (!(shipYardRef->constructShip(currentRow->shipToSpawn.Get(), shipCost, buildTime, popValue)))
	{
		startUnitBuildingOperation();
	}
}

//Determines a potential unit to build based on the unit weight
int AUnitManager::determineUnitToBuild()
{
	int total = 0;
	int cursor = 0;

	for (int i = 0; i < unitWeights.Num(); i++)
	{
		total += unitWeights[i];
	}

	int random = FMath::RandRange(0, total);

	for (int i = 0; i < unitWeights.Num(); i++)
	{
		cursor += unitWeights[i];
		if (cursor >= random)
		{
			return i;
		}
	}
	return 0;
}

//Finds the weight of the unit and returns it
int AUnitManager::findUnitWeightValue(FString rowName)
{
	currentRow = findCurrentRow(rowName);
	return currentRow->buildWeight;
}

FConstructionData* AUnitManager::findCurrentRow(FString rowName)
{
	FName rowToFind = FName(*rowName);
	return dataTableRef.DataTable->FindRow<FConstructionData>(rowToFind, "");
}