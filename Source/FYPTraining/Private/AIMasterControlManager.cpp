// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMasterControlManager.h"
#include "FYPTraining/FYPTrainingGameMode.h"
#include "ResourceManager.h"
#include <Kismet/GameplayStatics.h>
#include "SelectableObject.h"
#include "ResourceManager.h"
#include "ResearchManager.h"
#include "UnitManager.h"

// Sets default values
AAIMasterControlManager::AAIMasterControlManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAIMasterControlManager::Init(AFYPTrainingGameMode* gmRef, bool useDDS, bool useLanchester)
{
	gamemodeRef = gmRef;

	//AResourceManager* rm = Cast<AResourceManager>(rmRef);
	//if (rm)
	//{
	//	rm->Init(gamemodeRef);
	//}
	
	dynamicDifficultyScalingActive = useDDS;

	if (dynamicDifficultyScalingActive)
	{
		GetWorldTimerManager().SetTimer(ddsCheckTimer, this, &AAIMasterControlManager::dynamicDifficultyScalingCheck, 5.0f, true, 5.0f);
	}

	AActor* spawnedResourceManager = GetWorld()->SpawnActor(resourceManager);
	resourceManagerRef = Cast<AResourceManager>(spawnedResourceManager);
	if (resourceManagerRef)
	{
		resourceManagerRef->Init(gamemodeRef, useLanchester);
	}

	AActor* spawnedResearchManager = GetWorld()->SpawnActor(researchManager);
	researchManagerRef = Cast<AResearchManager>(spawnedResearchManager);
	if (researchManagerRef)
	{
		researchManagerRef;
	}

	AActor* spawnedUnitManager = GetWorld()->SpawnActor(unitManager);
	unitManagerRef = Cast<AUnitManager>(spawnedUnitManager);
	if (unitManagerRef)
	{
		unitManagerRef->Init(gamemodeRef);
	}
}

void AAIMasterControlManager::dynamicDifficultyScalingCheck()
{
	float s1 = compareUnitPower();
	float s2 = compareEconomy();

	float s = (s1 + s2) / 2;

	if (s >= 1)
	{
		s = 1;
	}
	else if (s <= 0)
	{
		s = 0;
	}

	gamemodeRef->currentaiDifficulty = s;
	gamemodeRef->aiIncomeMultiplier = s * (1.5f - 0.75f) + 0.75f;
}


float AAIMasterControlManager::compareEconomy()
{
	float AIEconWeight = gamemodeRef->aiIncomePerSecond / 195.0f;
	float PlayerEconWeight = gamemodeRef->playerIncomePerSecond / 195.0f;

	float econScore = PlayerEconWeight - AIEconWeight;
	
	return baseSRate + learningRate * econScore;
}

float AAIMasterControlManager::compareUnitPower()
{
	float score = 0;

	for (int i = 0; i < gamemodeRef->ActivePlayerShips.Num(); i++)
	{
		if (gamemodeRef->ActivePlayerShips.IsValidIndex(i))
		{
			ASelectableObject* shipRef = Cast<ASelectableObject>(gamemodeRef->ActivePlayerShips[i]);
			if (shipRef)
			{
				score = score + 1 + (shipRef->getUnitHealthPercentage() * 0.5f);
			}
		}
	}
	for (int i = 0; i < gamemodeRef->ActiveAiShips.Num(); i++)
	{
		if (gamemodeRef->ActiveAiShips.IsValidIndex(i))
		{
			ASelectableObject* shipRef = Cast<ASelectableObject>(gamemodeRef->ActiveAiShips[i]);
			if (shipRef)
			{
				score = score - 1 - (shipRef->getUnitHealthPercentage() * 0.5f);
			}
		}
	}

	return baseSRate + learningRate * score;
}
