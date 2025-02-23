// Written by Thomas Stevens, all rights reserved


#include "ResourceManager.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "CombatManager.h"
#include "Shipyard.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AResourceManager::AResourceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AResourceManager::Init(AFYPTrainingGameMode* gmRef, bool useLanchester)
{
	gamemodeRef = gmRef;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mine1Spawned = GetWorld()->SpawnActor<AActor>(mineClass, mine1SpawnLoc, GetActorRotation(), SpawnParams);
	mine2Spawned = GetWorld()->SpawnActor<AActor>(mineClass, mine2SpawnLoc, GetActorRotation(), SpawnParams);

	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShipyard::StaticClass(), foundActors);

	for (int i = 0; i < foundActors.Num(); i++)
	{
		if (foundActors.IsValidIndex(i))
		{
			AShipyard* shipyard = Cast<AShipyard>(foundActors[i]);
			if (shipyard)
			{
				if (!shipyard->playerControlled)
				{
					shipyardRef = shipyard;
				}
			}
		}
	}

	AActor* spawnedCombatManager = GetWorld()->SpawnActor(combatManager);
	cmRef = Cast<ACombatManager>(spawnedCombatManager);
	if (cmRef)
	{
		cmRef->Init(gmRef, this, mine1Spawned, mine2Spawned, useLanchester);
	}

	GetWorldTimerManager().SetTimer(mineStatusCheckTimer, this, &AResourceManager::checkMineStatus, 5.0f, true, 5.0f);
}

//Builds all available mines
void AResourceManager::triggerMineBuild()
{
	shipyardRef->buildMines();
}

//Checks if the player has more mines than the AI. If true, send a ship to capture a player credit mine
void AResourceManager::checkMineStatus()
{
	if (gamemodeRef->AIResourceMine.Num() < gamemodeRef->PlayerResourceMine.Num())
	{
		cmRef->taskCaptureMine();
	}
}