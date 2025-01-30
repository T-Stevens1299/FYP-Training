// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#include "FYPTrainingGameMode.h"
#include "FYPTrainingCharacter.h"
#include "PlayerHUD.h"
#include "Shipyard.h"
#include "ResourceMine.h"
#include "Blueprint/UserWidget.h"
#include <Kismet/GameplayStatics.h>
#include "UObject/ConstructorHelpers.h"
#include "AIMasterControlManager.h"
#include "GameEndScreen.h"
#include "ResourceManager.h"

AFYPTrainingGameMode::AFYPTrainingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_RTS_Camera"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AFYPTrainingGameMode::BeginPlay()
{
	Super::BeginPlay();

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AActor* spawnedAiManager = GetWorld()->SpawnActor(aiManager);
	aiManagerRef = Cast<AAIMasterControlManager>(spawnedAiManager);
	if (aiManagerRef)
	{
		aiManagerRef->Init(this, useDDS, useLanchester);
	}

	HUD = CreateWidget<UPlayerHUD>(PC, HUDref);
	HUD->SetGmPtr(this);

	currentPlayerMoney = startingPlayerFunds;

	currentAIMoney = (startingAiFunds * aiIncomeMultiplier);

	HUD->updateFunds(currentPlayerMoney);
	HUD->AddToViewport();

	GetWorldTimerManager().SetTimer(incomeHandle, this, &AFYPTrainingGameMode::IncreaseIncome, 1.0f, true, 1.0f);

	setShipyards();

	setMines();
}

void AFYPTrainingGameMode::IncreaseIncome()
{
	matchDuration++;
	currentAIMoney = currentAIMoney + (aiIncomePerSecond * aiIncomeMultiplier);
	currentPlayerMoney = currentPlayerMoney + playerIncomePerSecond;
	HUD->updateFunds(currentPlayerMoney);
}

void AFYPTrainingGameMode::increaseIncomePerSecond(bool playerControlled, float incomeToIncrease)
{
	if (playerControlled)
	{
		playerIncomePerSecond = playerIncomePerSecond + incomeToIncrease;
	}
	else
	{
		aiIncomePerSecond = aiIncomePerSecond + incomeToIncrease;
	}
}

void AFYPTrainingGameMode::subtractCost(bool playerControlled, float incomeToSubtract)
{
	if (playerControlled)
	{
		currentPlayerMoney = currentPlayerMoney - incomeToSubtract;
	}
	else
	{
		currentAIMoney = currentAIMoney - incomeToSubtract;
	}
}

void AFYPTrainingGameMode::setShipyards()
{
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShipyard::StaticClass(), foundActors);
	for(int i = 0; i < foundActors.Num(); i++)
	{
		if (foundActors.IsValidIndex(i))
		{
			AShipyard* foundShipyardRef = Cast<AShipyard>(foundActors[i]);
			if (foundShipyardRef)
			{
				foundShipyardRef->init(this);
			}
		}
	}
}

void AFYPTrainingGameMode::setMines()
{
	TArray<AActor*> foundMines;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResourceMine::StaticClass(), foundMines);
	for (int i = 0; i < foundMines.Num(); i++)
	{
		if (foundMines.IsValidIndex(i))
		{
			AResourceMine* foundMineRef = Cast<AResourceMine>(foundMines[i]);
			if (foundMineRef)
			{
				foundMineRef->Init(this);
			}
		}
	}
}

void AFYPTrainingGameMode::updateMineStatus(AActor* passedMine, bool playerControlled, bool isCaptured)
{
	if (isCaptured)
	{
		if (playerControlled)
		{
			PlayerResourceMine.Add(passedMine);
		}
		else 
		{
			AIResourceMine.Add(passedMine);
			aiManagerRef->resourceManagerRef->triggerMineBuild();
		}
	}
	else 
	{
		if (playerControlled)
		{
			for (int i = 0; PlayerResourceMine.Num(); i++)
			{
				if (passedMine == PlayerResourceMine[i])
				{
					PlayerResourceMine.RemoveSingle(AIResourceMine[i]);
				}
			}
		}
		else
		{
			for (int i = 0; AIResourceMine.Num(); i++)
			{
				if (passedMine == AIResourceMine[i])
				{
					AIResourceMine.RemoveSingle(AIResourceMine[i]);
				}
			}
		}
	}
}

void AFYPTrainingGameMode::addShipsToArray(AActor* shipToAdd, bool isPlayerControlled)
{
	if (isPlayerControlled)
	{
		ActivePlayerShips.Add(shipToAdd);
	}
	else
	{
		ActiveAiShips.Add(shipToAdd);
	}
}

void AFYPTrainingGameMode::removeShipsFromArray(AActor* shipToAdd, FString className, bool isPlayerControlled)
{
	if (isPlayerControlled)
	{
		ActivePlayerShips.RemoveSingle(shipToAdd);
		playerShipsLost.Add(className);
	}
	else
	{
		ActiveAiShips.RemoveSingle(shipToAdd);
		aiShipsLost.Add(className);
	}
}

void AFYPTrainingGameMode::updatePopCap(bool playerControlled, int popValue)
{
	if (playerControlled)
	{
		currentPlayerPopCap = currentPlayerPopCap + popValue;
	}
	else
	{
		currentAiPopCap = currentAiPopCap + popValue;
	}
}

void AFYPTrainingGameMode::gameEnd(bool playerControlled)
{
	HUD->RemoveFromParent();
	GameEnd = CreateWidget<UGameEndScreen>(PC, gameEndRef);
	GameEnd->AddToViewport();
	calculateLostShips();
	GameEnd->SetScreenText(playerControlled);
}

void AFYPTrainingGameMode::calculateLostShips()
{
	//Defining ints for player ship types
	int pCorvetteLost = 0;
	int aiCorvetteLost = 0;

	int pFrigateLost = 0;
	int aiFrigateLost = 0;

	int pDestroyLost = 0;
	int aiDestroyLost = 0;

	int pCruiserLost = 0;
	int aiCruiserLost = 0;

	int pBattleLost = 0;
	int aiBattleLost = 0;


	//Loop through and determine how many ships of each class were lost
	for (int i = 0; i < playerShipsLost.Num(); i++)
	{
		if (playerShipsLost[i].Contains("Corvette")) { pCorvetteLost++; }
		if (playerShipsLost[i].Contains("Frigate")) { pFrigateLost++; }
		if (playerShipsLost[i].Contains("Destroyer")) { pDestroyLost++; }
		if (playerShipsLost[i].Contains("Cruiser")) { pCruiserLost++; }
		if (playerShipsLost[i].Contains("Battleship")) { pBattleLost++; }
	}

	for (int i = 0; i < aiShipsLost.Num(); i++)
	{
		if (aiShipsLost[i].Contains("Corvette")) { aiCorvetteLost++; }
		if (aiShipsLost[i].Contains("Frigate")) { aiFrigateLost++; }
		if (aiShipsLost[i].Contains("Destroyer")) { aiDestroyLost++; }
		if (aiShipsLost[i].Contains("Cruiser")) { aiCruiserLost++; }
		if (aiShipsLost[i].Contains("Battleship")) { aiBattleLost++; }
	}

	int totalPlayerLoss = pCorvetteLost + pFrigateLost + pDestroyLost + pCruiserLost + pBattleLost;
	int totalAiLoss = aiCorvetteLost + aiFrigateLost + aiDestroyLost + aiCruiserLost + aiBattleLost;

	GameEnd->pCorvetteLoss = FText::FromString(FString::FromInt(pCorvetteLost));
	GameEnd->aiCorvetteLoss = FText::FromString(FString::FromInt(aiCorvetteLost));

	GameEnd->pFrigateLoss = FText::FromString(FString::FromInt(pFrigateLost));
	GameEnd->aiFrigateLoss = FText::FromString(FString::FromInt(aiFrigateLost));

	GameEnd->pDestroyerLoss = FText::FromString(FString::FromInt(pDestroyLost));
	GameEnd->aiDestroyerLoss = FText::FromString(FString::FromInt(aiDestroyLost));

	GameEnd->pCruiserLoss = FText::FromString(FString::FromInt(pCruiserLost));
	GameEnd->aiCruiserLoss = FText::FromString(FString::FromInt(aiCruiserLost));

	GameEnd->pBattleShipLoss = FText::FromString(FString::FromInt(pBattleLost));
	GameEnd->aiBattleShipLoss = FText::FromString(FString::FromInt(aiBattleLost));

	GameEnd->pLoss = FText::FromString(FString::FromInt(totalPlayerLoss));
	GameEnd->aiLoss = FText::FromString(FString::FromInt(totalAiLoss));

	GameEnd->matchLength = matchDuration;
}