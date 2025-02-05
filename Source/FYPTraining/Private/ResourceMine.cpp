// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceMine.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Hardpoint.h"
#include "ShipHealthBar.h"
#include "Components/ProgressBar.h"


AResourceMine::AResourceMine()
{
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMeshComponent"));
	MineMesh->SetupAttachment(RootComponent);

	CaptureSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("CaptureSensorComponent"));
	CaptureSensor->SetupAttachment(MineMesh);

	hardpointSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("hardpointSpawnComponent"));
	hardpointSpawn->SetupAttachment(MineMesh);
}

void AResourceMine::Init(AFYPTrainingGameMode* gamemodeReference)
{
	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	gmRef = gamemodeReference;

	playerControlled = false;

	GetWorldTimerManager().SetTimer(captureTriggerTimer, this, &AResourceMine::captureMine, 1, true, 1);
}

void AResourceMine::generateIncome(float newIncome, bool techUpgrade)
{
	if (techUpgrade)
	{
		float incomeToAdd = newIncome - IncomeRate;
		UE_LOG(LogTemp, Warning, TEXT("MoneyToAdd: %d"), (int)incomeToAdd);
		gmRef->increaseIncomePerSecond(playerControlled, incomeToAdd);
		IncomeRate = newIncome;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InitNewMine"));
		gmRef->increaseIncomePerSecond(playerControlled, IncomeRate);
	}
}

void AResourceMine::setMineLevel(int newMineLevel)
{
	int arrayPosition = newMineLevel - 2;
	if (IncomeRateLevels.IsValidIndex(arrayPosition))
	{
		generateIncome(IncomeRateLevels[arrayPosition], true);
	}
}

void AResourceMine::captureMine()
{
	if (!isCaptured)
	{
		playerShipsCapturing = 0;
		enemyShipsCapturing = 0;

		TArray<AActor*> overlappingShips;
		CaptureSensor->GetOverlappingActors(overlappingShips);

		for (int i = 0; i < overlappingShips.Num(); i++)
		{
			ASelectableObject* shipToCheck = Cast<ASelectableObject>(overlappingShips[i]);
			if (shipToCheck)
			{
				if (!(shipToCheck == this))
				{
					if (shipToCheck->playerControlled)
					{
						playerShipsCapturing++;
					}
					else
					{
						enemyShipsCapturing++;
					}
				}
			}
		}

		if (playerShipsCapturing == 0 && enemyShipsCapturing > 0)
		{
			if (!GetWorldTimerManager().IsTimerActive(captureTimer))
			{			
				GetWorldTimerManager().SetTimer(captureTimer, this, &AResourceMine::captureMineSequence, 1.0f, true, 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("StartTime"));
				playerControlled = false;
			}
		}
		else if (enemyShipsCapturing == 0 && playerShipsCapturing > 0)
		{
			if (!GetWorldTimerManager().IsTimerActive(captureTimer))
			{
				GetWorldTimerManager().SetTimer(captureTimer, this, &AResourceMine::captureMineSequence, 1.0f, true, 1.0f);
				UE_LOG(LogTemp, Warning, TEXT("StartTime"));
				playerControlled = true;
			}
		}
		else
		{
			if (GetWorldTimerManager().IsTimerActive(captureTimer))
			{
				GetWorldTimerManager().ClearTimer(captureTimer);
				UE_LOG(LogTemp, Warning, TEXT("StopTimer"));
			}
		}
	}
}

void AResourceMine::captureMineSequence()
{
	curCaptureTime++;
	healthBarRef->updateHealthBar(curCaptureTime / captureTime);
	UE_LOG(LogTemp, Warning, TEXT("Capture Time: %d"), (int)curCaptureTime);
	if (curCaptureTime != captureTime) { return; }

	isCaptured = true;
	gmRef->updateMineStatus(this, playerControlled, isCaptured);

	GetWorldTimerManager().ClearTimer(captureTimer);
	healthBarRef->updateHealthBar(0.0f);
}

void AResourceMine::buildMine()
{
	UE_LOG(LogTemp, Warning, TEXT("MineBuilt"));

	FActorSpawnParameters spawnParams;
	UChildActorComponent* hardpoint = NewObject<UChildActorComponent>(this);
	hardpoint->RegisterComponent();
	hardpoint->SetChildActorClass(hardpointSpawnRef);
	hardpoint->CreateChildActor();
	
	if (playerControlled) { MineMesh->SetMaterial(0, playerMaterial); healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Green); }
	else { MineMesh->SetMaterial(0, enemyMaterial); healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Red); }

	generateIncome(0, false);
	isBuilt = true;

	GetWorldTimerManager().SetTimer(delayTimer, this, &AResourceMine::hardpointCheckDelay, 2.0f, true, 2.0f);
}

void AResourceMine::hardpointCheckDelay()
{
	setHardpointsParent();
	GetWorldTimerManager().ClearTimer(delayTimer);
}

void AResourceMine::HealthCalculations()
{
	float newTotalHealth = 0.0f;
	AHardpoint* curHardpointRef;

	for (int i = 0; i < Hardpoints.Num(); i++)
	{
		curHardpointRef = Cast<AHardpoint>(Hardpoints[i]);
		if (curHardpointRef)
		{
			newTotalHealth = newTotalHealth + curHardpointRef->currentHealth;
		}
	}

	if (initHealthCheck)
	{
		totalUnitHealth = newTotalHealth;
		initHealthCheck = false;
	}

	currentUnitHealth = newTotalHealth;

	float percent = currentUnitHealth / totalUnitHealth;

	if (IsValid(healthBarRef)) { healthBarRef->updateHealthBar(percent); }

	UE_LOG(LogTemp, Warning, TEXT("Overriden The Total unit health is: %d"), (int)newTotalHealth);

	if (newTotalHealth <= 0)
	{
		isBuilt = false;
		isCaptured = false;

		if (playerControlled)
		{
			UE_LOG(LogTemp, Warning, TEXT("MineBuilt"));
			gmRef->PlayerResourceMine.RemoveSingle(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RemoveMine"));
			gmRef->AIResourceMine.RemoveSingle(this);
		}

		healthBarRef->HealthBar->SetFillColorAndOpacity(FLinearColor::Yellow);
	}
}