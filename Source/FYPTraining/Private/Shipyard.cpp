// Fill out your copyright notice in the Description page of Project Settings.


#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.h"
#include <Kismet/GameplayStatics.h>

AShipyard::AShipyard()
{
	unitSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("UnitSpawnPosition"));
	unitSpawner->SetupAttachment(RootComponent);
}

void AShipyard::init(AFYPTrainingGameMode* gamemodeReference)
{
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	gmRef = gamemodeReference;

	GetWorldTimerManager().SetTimer(incomeTimer, this, &AShipyard::GenerateBaseIncome, 1.0f, true, 1.0f);

	HUD = CreateWidget<UShipyardWidget>(PC, HUDref);
	HUD->init(this);
	HUD->AddToViewport();
}

void AShipyard::GenerateBaseIncome()
{
	gmRef->IncreaseIncome(playerControlled, baseIncomeRate);
}

void AShipyard::constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime)
{
	//Checks if there is already something constructing
	if (!isConstructingAlready)
	{
		//Detemines if the unit can be built
		int fundCheck = (gmRef->currentPlayerMoney - shipCost);
		if (fundCheck >= 0)
		{
			shipConstructing = shipToSpawn;
			isConstructingAlready = true;
			gmRef->IncreaseIncome(playerControlled, -shipCost);
			UE_LOG(LogTemp, Warning, TEXT("Construction Time Again"));
			GetWorldTimerManager().SetTimer(constructionTime, this, &AShipyard::buildShip, buildTime, true, buildTime);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Money :("));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Constructing RN"));
	}
}

void AShipyard::buildShip()
{
	//Set Spawn params
	FActorSpawnParameters spawnParams;

	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector spawnLoc = unitSpawner->GetComponentLocation();
	FRotator spawnRot = unitSpawner->GetComponentRotation();

	AActor* spawnedShip = GetWorld()->SpawnActor<AActor>(shipConstructing, spawnLoc, spawnRot, spawnParams);

	//Sets the faction of the spawned ship
	ASelectableObject* classRef = Cast<ASelectableObject>(spawnedShip);
	if (classRef)
	{
		classRef->playerControlled = playerControlled;
	}

	UE_LOG(LogTemp, Warning, TEXT("Construction Finished"));
	isConstructingAlready = false;
	GetWorldTimerManager().ClearTimer(constructionTime);
}

void AShipyard::canUpgradeTechLevel(float upgradeCost, float upgradeTime)
{
	if (!isUpgradingAlready)
	{
		int fundsCheck = (gmRef->currentPlayerMoney - upgradeCost);
		if (fundsCheck >= 0)
		{
			isUpgradingAlready = true;
			gmRef->IncreaseIncome(playerControlled, -upgradeCost);
			GetWorldTimerManager().SetTimer(upgradingTimer, this, &AShipyard::upgradeLevel, upgradeTime, true, upgradeTime);
			UE_LOG(LogTemp, Warning, TEXT("Upgrading Time Again"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Money :("));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrading RN"));
	}
}

void AShipyard::upgradeLevel()
{
	isUpgradingAlready = false;
	GetWorldTimerManager().ClearTimer(upgradingTimer);
	HUD->upgradeTechLevel();
}