// Fill out your copyright notice in the Description page of Project Settings.


#include "Shipyard.h"
#include "FYPTraining/FYPTrainingGameMode.h"

void AShipyard::init(AFYPTrainingGameMode* gamemodeReference)
{
	UE_LOG(LogTemp, Warning, TEXT("InitBruv"));
	gmRef = gamemodeReference;
	GetWorldTimerManager().SetTimer(incomeTimer, this, &AShipyard::GenerateBaseIncome, 1.0f, true, 1.0f);
}

void AShipyard::GenerateBaseIncome()
{
	UE_LOG(LogTemp, Warning, TEXT("IncomeGenerated"));
	gmRef->IncreaseIncome(playerControlled, baseIncomeRate);
}