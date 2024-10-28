// Copyright Stevens Studios, all rights reserved
// Written by Thomas Stevens

#include "FYPTrainingGameMode.h"
#include "FYPTrainingCharacter.h"
#include "PlayerHUD.h"
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
}

void AFYPTrainingGameMode::IncreaseIncome(bool isAiControlled, float moneyToAdd)
{
	if (isAiControlled)
	{
		currentAIMoney = currentAIMoney + (moneyToAdd * aiIncomeMultiplier);
	}
	else 
	{
		currentPlayerMoney = currentPlayerMoney + moneyToAdd;
		HUD->updateFunds(currentPlayerMoney);
	}
}