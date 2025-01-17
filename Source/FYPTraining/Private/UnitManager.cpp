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
}

void AUnitManager::setShipyardPtr(AShipyard* shipyardRef)
{
	shipYardRef = shipyardRef;
	UE_LOG(LogTemp, Warning, TEXT("Shipyard Ref Set"));
}

// Called when the game starts or when spawned
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}