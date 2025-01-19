// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManager.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "SelectableObject.h"


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

void ACombatManager::captureInitialMines(AFYPTrainingGameMode* gmRef, AActor* passedMine1, AActor* passedMine2)
{
	gamemodeRef = gmRef;

	AActor* firstShipToOrder = GetWorld()->SpawnActor(corvetteSpawnRef);
	AActor* secondShipToOrder = GetWorld()->SpawnActor(corvetteSpawnRef);

	//gamemodeRef->ActiveAiShips.Add(firstShipToOrder);
	//gamemodeRef->ActiveAiShips.Add(secondShipToOrder);

	//ASelectableObject* firstShip = Cast<ASelectableObject>(firstShipToOrder);
	//if (firstShip)
	//{
	//	firstShip->moveObject(passedMine1->GetActorLocation(), firstShip->WeaponsRange);
	//}

	//ASelectableObject* secondShip = Cast<ASelectableObject>(secondShipToOrder);
	//if (secondShip)
	//{
	//	secondShip->moveObject(passedMine2->GetActorLocation(), secondShip->WeaponsRange);
	//}

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
		//UE_LOG(LogTemp, Warning, TEXT("AttackOrder"));
	}
	else
	{
		//Add functionality to allow ships to stay idle
		taskOrderUnit(1);
		//UE_LOG(LogTemp, Warning, TEXT("RetreatOrder"));
	}
}

void ACombatManager::taskCaptureMine()
{

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
				UE_LOG(LogTemp, Warning, TEXT("Passed Order Is: %i"), passedOrderCode);
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