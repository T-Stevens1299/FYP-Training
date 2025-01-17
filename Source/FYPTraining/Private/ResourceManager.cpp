// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "CombatManager.h"

// Sets default values
AResourceManager::AResourceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AResourceManager::Init(AFYPTrainingGameMode* gmRef)
{
	gamemodeRef = gmRef;

	AActor* spawnedCombatManager = GetWorld()->SpawnActor(combatManager);
	cmRef = Cast<ACombatManager>(spawnedCombatManager);
	if (cmRef)
	{
		cmRef->captureInitialMines(gmRef, mine1, mine2);
	}
}

// Called every frame
void AResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

