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

	initBlueprintScript();

	//Enemy shipyard locates the combat manager and sends a reference of its self to it
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnitManager::StaticClass(), foundActors);
	for (int i = 0; i < foundActors.Num(); i++)
	{
		if (foundActors.IsValidIndex(i))
		{
			managerRef = Cast<AUnitManager>(foundActors[i]);
			if (managerRef)
			{
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
					managerRef->setShipyardPtr(this); 
				}
			}			
		}
	}

	setHardpointsParent();
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

bool AShipyard::constructShip(TSubclassOf<AActor> shipToSpawn, float shipCost, float buildTime, int popValue)
{
	//Checks if there is already something constructing
	if (!isConstructingAlready)
	{
		//Detemines if the unit can be built
		int fundCheck;

		if (playerControlled)
		{ 
			if (gmRef->currentPlayerPopCap + popValue <= gmRef->playerPopCap)
			{
				fundCheck = (gmRef->currentPlayerMoney - shipCost); 
			}
			else
			{
				fundCheck = -1;
			}
		}
		else
		{
			if (gmRef->currentAiPopCap + popValue <= gmRef->aiPopCap)
			{
				fundCheck = (gmRef->currentAIMoney - shipCost);
			}
			else
			{
				fundCheck = -1;
			}
		}

		if (fundCheck >= 0)
		{
			currentShipCost = shipCost;
			shipConstructing = shipToSpawn;
			currentShipPopValue = popValue;
			isConstructingAlready = true;
			gmRef->subtractCost(playerControlled, shipCost);
			UE_LOG(LogTemp, Warning, TEXT("Construction Time Again"));
			GetWorldTimerManager().SetTimer(constructionTime, this, &AShipyard::buildShip, buildTime, true, buildTime);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Money :("));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Constructing RN"));
		return false;
	}
}

void AShipyard::buildShip()
{
	AActor* spawnedShip = spawnShip(shipConstructing);
	//Sets the faction of the spawned ship
	ASelectableObject* classRef = Cast<ASelectableObject>(spawnedShip);
	if (classRef)
	{
		gmRef->addShipsToArray(spawnedShip, playerControlled);
		gmRef->updatePopCap(playerControlled, currentShipPopValue);

		classRef->initaliseSelectableObject(playerControlled, currentShipCost, currentShipPopValue);

		if (!playerControlled)
		{
			classRef->retreatPointRef = retreatPoint;
			classRef->attackPointRef = attackPoint;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Construction Finished"));
	isConstructingAlready = false;
	GetWorldTimerManager().ClearTimer(constructionTime);
}

AActor* AShipyard::spawnShip(TSubclassOf<AActor> shipToSpawn)
{
	//Set Spawn params
	FActorSpawnParameters spawnParams;

	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector spawnLoc = unitSpawner->GetComponentLocation();
	FRotator spawnRot = unitSpawner->GetComponentRotation();

	return GetWorld()->SpawnActor<AActor>(shipToSpawn, spawnLoc, spawnRot, spawnParams);
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

bool AShipyard::canUpgradeTechLevel(float upgradeCost, float upgradeTime)
{
	if (!isUpgradingAlready)
	{
		int fundsCheck = (gmRef->currentPlayerMoney - upgradeCost);

		if (fundsCheck >= 0)
		{
			isUpgradingAlready = true;
			gmRef->subtractCost(playerControlled, upgradeCost);
			GetWorldTimerManager().SetTimer(upgradingTimer, this, &AShipyard::upgradeLevel, upgradeTime, true, upgradeTime);
			UE_LOG(LogTemp, Warning, TEXT("Upgrading Time Again"));
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Money :("));
			return false;
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Upgrading RN"));
		return false;
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