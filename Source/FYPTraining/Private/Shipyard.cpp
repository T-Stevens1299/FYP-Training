// Fill out your copyright notice in the Description page of Project Settings.


#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ShipyardWidget.h"
#include <Kismet/GameplayStatics.h>
#include "ResourceMine.h"
#include "UnitManager.h"
#include "ShipHealthBar.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

AShipyard::AShipyard()
{
	unitSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("UnitSpawnPosition"));
	unitSpawner->SetupAttachment(RootComponent);
}

void AShipyard::init(AFYPTrainingGameMode* gamemodeReference)
{

	gmRef = gamemodeReference;

	//healthBarRef = Cast<UShipHealthBar>(healthBar->GetUserWidgetObject());

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

	spawnStartingShips();

	GetWorldTimerManager().SetTimer(healthBarDelay, this, &AShipyard::healthBarSet, 1.0f, true, 1.0f);
}

void AShipyard::healthBarSet()
{
	GetWorldTimerManager().ClearTimer(healthBarDelay);
	if(playerControlled) { healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Green); }
	else { healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Red); }
	HealthCalculations();
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
			currentShipPopValue = popValue;

			switch (currentShipPopValue)
			{
			case 10:
				shipConstructing = corvetteRef;
				break;
			case 20:
				shipConstructing = frigateRef;
				break;
			case 25:
				shipConstructing = destroyerRef;
				break;
			case 30:
				shipConstructing = cruiserRef;
				break;
			case 40:
				shipConstructing = battleshipRef;
				break;
			default:
				shipConstructing = corvetteRef;
				break;
			}

			isConstructingAlready = true;
			curShipConTime = buildTime;
			constructionProgress = 0.0f;
			gmRef->subtractCost(playerControlled, shipCost);
			UE_LOG(LogTemp, Warning, TEXT("Construction Time Again"));
			GetWorldTimerManager().SetTimer(constructionTime, this, &AShipyard::buildShipProgress, 1.0f, true, 1.0f);
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

void AShipyard::spawnStartingShips()
{
	shipConstructing = startingShipRef;
	currentShipPopValue = 10;
	for (int i = 0; i < startingShipCount; i++)
	{
		buildShip();
	}
}

void AShipyard::buildShipProgress()
{
	constructionProgress++;
	if (playerControlled)
	{
		HUD->updateConstructionBar(constructionProgress / curShipConTime);
		if (constructionProgress != curShipConTime) { return; }
		buildShip();
		HUD->updateConstructionBar(0.0f);
	}
	else
	{
		if (constructionProgress != curShipConTime) { return; }
		buildShip();
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
		if (curMoney >= (mineCost * mineRef.Num()) || !playerControlled)
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
							if (playerControlled) { HUD->updateMineCount(false); gmRef->subtractCost(playerControlled, mineCost); }
							UE_LOG(LogTemp, Warning, TEXT("BuildMine"));
							curMine->buildMine();
						}
					}
				}
			}
		}
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
			curTechUpgradeTime = upgradeTime;
			techUpgradeProgress = 0.0f;
			gmRef->subtractCost(playerControlled, upgradeCost);
			GetWorldTimerManager().SetTimer(upgradingTimer, this, &AShipyard::upgradeLevelProgress, 1.0f, true, 1.0f);
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

void AShipyard::upgradeLevelProgress()
{
	techUpgradeProgress++;
	HUD->updateTechBar(techUpgradeProgress / curTechUpgradeTime);
	if (techUpgradeProgress != curTechUpgradeTime) { return; }
	upgradeLevel();
}

void AShipyard::upgradeLevel()
{
	isUpgradingAlready = false;
	GetWorldTimerManager().ClearTimer(upgradingTimer);
	HUD->updateTechBar(0.0f);
	HUD->upgradeTechLevel();
}

void AShipyard::triggerWinCheck()
{
	gmRef->gameEnd(playerControlled);
	this->Destroy(true);
}