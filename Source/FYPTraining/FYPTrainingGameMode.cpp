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
		aiManagerRef->Init(this);
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

void AFYPTrainingGameMode::removeShipsFromArray(AActor* shipToAdd, bool isPlayerControlled)
{
	if (isPlayerControlled)
	{
		ActivePlayerShips.RemoveSingle(shipToAdd);
	}
	else
	{
		ActiveAiShips.RemoveSingle(shipToAdd);
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
	if (playerControlled)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Wins"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Wins"));
	}
}