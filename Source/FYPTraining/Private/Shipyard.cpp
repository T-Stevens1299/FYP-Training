// Fill out your copyright notice in the Description page of Project Settings.


#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.h"
#include <Kismet/GameplayStatics.h>
#include "ResourceMine.h"
#include "UnitManager.h"

AShipyard::AShipyard()
{
	unitSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("UnitSpawnPosition"));
	unitSpawner->SetupAttachment(RootComponent);
}

void AShipyard::init(AFYPTrainingGameMode* gamemodeReference)
{

	gmRef = gamemodeReference;

	generateIncome(0, false);

	if (playerControlled)
	{
		//Player shipyard sets the Player controller and creates a HUD interface
		PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		HUD = CreateWidget<UShipyardWidget>(PC, HUDref);
		HUD->init(this);
		HUD->AddToViewport();
	}
	else
	{
		//Enemy shipyard locates the combat manager and sends a reference of its self to it
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnitManager::StaticClass(), foundActors);
		for (int i = 0; i < foundActors.Num(); i++)
		{
			if (foundActors.IsValidIndex(i))
			{
				AUnitManager* managerRef = Cast<AUnitManager>(foundActors[i]);
				if (managerRef)
				{
					managerRef->setShipyardPtr(this);
				}
			}
		}
	}
}

void AShipyard::generateIncome(float prevIncomeRate, bool techUpgrade)
{
	if (techUpgrade)
	{
		float incomeToAdd = IncomeRate - prevIncomeRate;
		gmRef->increaseIncomePerSecond(playerControlled, incomeToAdd);
	}
	else
	{
		gmRef->increaseIncomePerSecond(playerControlled, IncomeRate);
	}
}

void AShipyard::constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime)
{
	//Checks if there is already something constructing
	if (!isConstructingAlready)
	{
		//Detemines if the unit can be built
		int fundCheck;

		if (playerControlled) { fundCheck = (gmRef->currentPlayerMoney - shipCost); }
		if (!playerControlled) { fundCheck = (gmRef->currentAIMoney - shipCost); }

		if (fundCheck >= 0)
		{
			currentShipCost = shipCost;
			shipConstructing = shipToSpawn;
			isConstructingAlready = true;
			gmRef->subtractCost(playerControlled, shipCost);
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
		gmRef->addShipsToArray(classRef, playerControlled);
		classRef->playerControlled = playerControlled;
		classRef->unitCost = currentShipCost;
	}

	UE_LOG(LogTemp, Warning, TEXT("Construction Finished"));
	isConstructingAlready = false;
	GetWorldTimerManager().ClearTimer(constructionTime);
}

void AShipyard::buildMines()
{
	TArray<AActor*> mineRef;
	AResourceMine* curMine;
	float curMoney;

	//Seperate grabs depending on shipyard type
	if (playerControlled) { mineRef = gmRef->PlayerResourceMine; curMoney = gmRef->currentPlayerMoney; }
	if (!playerControlled) { mineRef = gmRef->AIResourceMine; curMoney = gmRef->currentAIMoney; }

	if (!(mineRef.Num() == 0))
	{
		if (curMoney >= (mineCost * mineRef.Num()))
		{
			for (int i = 0; i < mineRef.Num(); i++)
			{
				if (mineRef.IsValidIndex(i))
				{
					curMine = Cast<AResourceMine>(mineRef[i]);
					if (curMine)
					{
						if (!(curMine->isBuilt))
						{
							gmRef->subtractCost(playerControlled, mineCost);
							curMine->buildMine();
						}
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Active Mines"));
	}
}

void AShipyard::canUpgradeTechLevel(float upgradeCost, float upgradeTime)
{
	if (!isUpgradingAlready)
	{
		int fundsCheck;

		if (playerControlled) { fundsCheck = (gmRef->currentPlayerMoney - upgradeCost); }
		if (!playerControlled) { fundsCheck = (gmRef->currentAIMoney - upgradeCost); }

		if (fundsCheck >= 0)
		{
			isUpgradingAlready = true;
			gmRef->subtractCost(playerControlled, upgradeCost);
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

void AShipyard::triggerWinCheck()
{
	gmRef->gameEnd(playerControlled);
	this->Destroy(true);
}