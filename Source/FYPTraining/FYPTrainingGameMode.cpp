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

	HUD = CreateWidget<UPlayerHUD>(PC, HUDref);
	HUD->SetGmPtr(this);

	currentPlayerMoney = startingPlayerFunds;

	currentAIMoney = (startingAiFunds * aiIncomeMultiplier);

	HUD->updateFunds(currentPlayerMoney);
	HUD->AddToViewport();

	setShipyards();

	setMines();
}

void AFYPTrainingGameMode::IncreaseIncome(bool isAiControlled, float moneyToAdd)
{
	if (!isAiControlled)
	{
		currentAIMoney = currentAIMoney + (moneyToAdd * aiIncomeMultiplier);
	}
	else 
	{
		currentPlayerMoney = currentPlayerMoney + moneyToAdd;
		HUD->updateFunds(currentPlayerMoney);
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