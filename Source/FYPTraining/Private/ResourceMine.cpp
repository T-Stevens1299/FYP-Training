// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceMine.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"
#include "Hardpoint.h"

AResourceMine::AResourceMine()
{
	MineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMeshComponent"));
	MineMesh->SetupAttachment(RootComponent);

	CaptureSensor = CreateDefaultSubobject<UBoxComponent>(TEXT("CaptureSensorComponent"));
	CaptureSensor->SetupAttachment(MineMesh);
}

void AResourceMine::Init(AFYPTrainingGameMode* gamemodeReference)
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	gmRef = gamemodeReference;

	playerControlled = false;
}

void AResourceMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	captureMine();
}

void AResourceMine::generateIncome()
{
	gmRef->IncreaseIncome(playerControlled, IncomeRate);
}

void AResourceMine::setMineLevel(int newMineLevel)
{
	if (IncomeRateLevels.IsValidIndex(newMineLevel))
	{
		IncomeRate = IncomeRateLevels[newMineLevel];
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
				GetWorldTimerManager().SetTimer(captureTimer, this, &AResourceMine::captureMineSequence, captureTime, true, captureTime);
				UE_LOG(LogTemp, Warning, TEXT("StartTime"));
				playerControlled = false;
			}
		}
		else if (enemyShipsCapturing == 0 && playerShipsCapturing > 0)
		{
			if (!GetWorldTimerManager().IsTimerActive(captureTimer))
			{
				GetWorldTimerManager().SetTimer(captureTimer, this, &AResourceMine::captureMineSequence, captureTime, true, captureTime);
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
	isCaptured = true;
	if (playerControlled)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCapturedMine"));
		gmRef->updateMineStatus(this, playerControlled, isCaptured);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCapturedMine"));
		gmRef->updateMineStatus(this, playerControlled, isCaptured);
	}
	GetWorldTimerManager().ClearTimer(captureTimer);
}

void AResourceMine::buildMine()
{
	isBuilt = true;
	UE_LOG(LogTemp, Warning, TEXT("MineBuilt"));

	FActorSpawnParameters spawnParams;
	AActor* hardpoint = GetWorld()->SpawnActor<AHardpoint>(hardpointSpawnRef, GetActorLocation(), GetActorRotation(), spawnParams);
	Hardpoints.Add(hardpoint);

	setHardpointsParent();

	GetWorldTimerManager().SetTimer(incomeTimer, this, &AResourceMine::generateIncome, 1.0f, true, 1.0f);
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

	UE_LOG(LogTemp, Warning, TEXT("Overriden The Total unit health is: %d"), (int)newTotalHealth);

	if (newTotalHealth <= 0)
	{
		if (playerControlled)
		{
			gmRef->PlayerResourceMine.RemoveSingle(this);
		}
		else
		{
			gmRef->AIResourceMine.RemoveSingle(this);
		}
		isBuilt = false;
		isCaptured = false;
	}
}