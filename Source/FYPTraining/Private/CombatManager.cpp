// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "SelectableObject.h"
#include "ResourceManager.h"
#include "Shipyard.h"


// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACombatManager::Init()
{
	initStageComplete = true;

	GetWorldTimerManager().SetTimer(combatLoopTimer, this, &ACombatManager::selectorCaptureMineOrder, 2, true, 2);
}

void ACombatManager::captureInitialMines(AFYPTrainingGameMode* gmRef, AResourceManager* rmRef, AActor* passedMine1, AActor* passedMine2)
{
	gamemodeRef = gmRef;

	resourceManRef = rmRef;

	AActor* firstShipToOrder = resourceManRef->shipyardRef->spawnShip(corvetteSpawnRef);
	AActor* secondShipToOrder = resourceManRef->shipyardRef->spawnShip(corvetteSpawnRef);

	gamemodeRef->ActiveAiShips.Add(firstShipToOrder);
	gamemodeRef->ActiveAiShips.Add(secondShipToOrder);

	gamemodeRef->currentAiPopCap += 20.0f;

	ASelectableObject* firstShip = Cast<ASelectableObject>(firstShipToOrder);
	if (firstShip)
	{
		firstShip->retreatPointRef = resourceManRef->shipyardRef->retreatPoint;
		firstShip->attackPointRef = resourceManRef->shipyardRef->attackPoint;
		firstShip->initaliseSelectableObject(false, 750, 10);
		firstShip->moveObject(passedMine1->GetActorLocation(), 100);
	}

	ASelectableObject* secondShip = Cast<ASelectableObject>(secondShipToOrder);
	if (secondShip)
	{
		secondShip->retreatPointRef = resourceManRef->shipyardRef->retreatPoint;
		secondShip->attackPointRef = resourceManRef->shipyardRef->attackPoint;
		secondShip->initaliseSelectableObject(false, 750, 10);
		secondShip->moveObject(passedMine2->GetActorLocation(), 100);
	}

	Init();
}

void ACombatManager::selectorCaptureMineOrder()
{
	if (captureMine)
	{
		taskCaptureMine();
	}
	else
	{
		selectorCombatPredictionAlgorithm();
	}
}

void ACombatManager::selectorCombatPredictionAlgorithm()
{
	if (usingLanchesterModel)
	{
		shouldAttack = LanchesterModelAlgorithm();
	}
	else
	{
		shouldAttack = BasicCombatPredictionAlgorithm();
	}

	if (shouldAttack)
	{
		taskOrderUnit(2);
	}
	else
	{
		taskOrderUnit(1);
	}
}

void ACombatManager::taskCaptureMine()
{
	//Selects rnadom ship out of AI pool and sends it to destroy/capture a player resource mine.
	int totalAIShips = gamemodeRef->ActiveAiShips.Num() - 1;
	int shipToChoose = FMath::RandRange(0, totalAIShips);
	ASelectableObject* chosenShip = Cast<ASelectableObject>(gamemodeRef->ActiveAiShips[shipToChoose]);
	if (chosenShip)
	{
		chosenShip->moveToAttackTarget(gamemodeRef->PlayerResourceMine.Last(), 100);
	}
}

void ACombatManager::taskOrderUnit(int passedOrderCode)
{
	for (int i = 0; i < gamemodeRef->ActiveAiShips.Num(); i++)
	{
		if (gamemodeRef->ActiveAiShips.IsValidIndex(i)) 
		{
			ASelectableObject* curShip = Cast<ASelectableObject>(gamemodeRef->ActiveAiShips[i]);
			if (curShip)
			{
				curShip->orderCode = passedOrderCode;
				//UE_LOG(LogTemp, Warning, TEXT("Passed Order Is: %i"), passedOrderCode);
			}
		}
	}
}

//Combat prediction algorithm based on the Lanchester Model
bool ACombatManager::LanchesterModelAlgorithm()
{
	float aiArmyStrength = calculateArmyStrength(true);
	float playerArmyStremgth = calculateArmyStrength(false);

	float aiArmySize = gamemodeRef->ActiveAiShips.Num();
	float playerArmySize = gamemodeRef->ActivePlayerShips.Num();

	float aiOutcome = aiArmyStrength * (aiArmySize * aiArmySize);
	float playerOutcome = playerArmyStremgth * (playerArmySize * playerArmySize);

	if (aiOutcome >= playerOutcome)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float ACombatManager::calculateArmyStrength(bool calculateAIStrength)
{
	if (calculateAIStrength)
	{
		float AIArmyStrength = 0;

		for (int i = 0; i < gamemodeRef->ActiveAiShips.Num(); i++)
		{
			if (gamemodeRef->ActiveAiShips.IsValidIndex(i))
			{ 
				ASelectableObject* shipRef = Cast<ASelectableObject>(gamemodeRef->ActiveAiShips[i]);
				if (shipRef)
				{
					AIArmyStrength = AIArmyStrength + shipRef->getStrengthValue();
				}
			}
		}
		return AIArmyStrength;
	}
	else
	{
		float PlayerArmyStrength = 0;

		for (int i = 0; i< gamemodeRef->ActivePlayerShips.Num(); i++)
		{
			if (gamemodeRef->ActiveAiShips.IsValidIndex(i))
			{
				ASelectableObject* shipRef = Cast<ASelectableObject>(gamemodeRef->ActivePlayerShips[i]);
				if (shipRef)
				{
					PlayerArmyStrength = PlayerArmyStrength + shipRef->getStrengthValue();
				}
			}
		}
		return PlayerArmyStrength;
	}
}

//Sends units to attack if the number of units for both factions is the same or the AI has more
bool ACombatManager::BasicCombatPredictionAlgorithm()
{
	if (gamemodeRef->ActiveAiShips.Num() >= gamemodeRef->ActivePlayerShips.Num())
	{
		return true;
	}
	else
	{
		return false;
	}
}