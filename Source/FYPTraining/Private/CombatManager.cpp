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

void ACombatManager::captureInitialMines()
{
	GetWorldTimerManager().ClearTimer(initDelay);

	TArray<AActor*> aiShipArray = gamemodeRef->ActiveAiShips;

	for (int i = 0; i < aiShipArray.Num(); i++)
	{
		ASelectableObject* shipRef = Cast<ASelectableObject>(aiShipArray[i]);
		if (shipRef)
		{
			shipRef->moveObject(mineRefArray[i]->GetActorLocation(), 100);
		}
	}

	initStageComplete = true;

	GetWorldTimerManager().SetTimer(combatLoopTimer, this, &ACombatManager::selectorCaptureMineOrder, 1, true, 1);
}

void ACombatManager::Init(AFYPTrainingGameMode* gmRef, AResourceManager* rmRef, AActor* passedMine1, AActor* passedMine2, bool useLanchester)
{

	gamemodeRef = gmRef;

	resourceManRef = rmRef;

	mineRefArray.Add(passedMine2);
	mineRefArray.Add(passedMine1);

	usingLanchesterModel = useLanchester;

	GetWorldTimerManager().SetTimer(initDelay, this, &ACombatManager::captureInitialMines, 2, true, 2);
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

	if (!shouldAttack) { taskOrderUnit(1); }
	else
	{
		//If there are more than 5 ships been built since last cylce, let them attack
		if (shipsConstructedThisCycle > 4) { taskOrderUnit(2); shipsConstructedThisCycle = 0; }
		else { taskOrderUnit(0); }
	}
}

void ACombatManager::taskCaptureMine()
{
	//Selects rnadom ship out of AI pool and sends it to destroy/capture a player resource mine.
	if (gamemodeRef->ActiveAiShips.Num() == 0) { return; }
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