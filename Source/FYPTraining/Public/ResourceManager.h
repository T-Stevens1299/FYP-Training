// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceManager.generated.h"

class AFYPTrainingGameMode;
class ACombatManager;
class AShipyard;

UCLASS()
class FYPTRAINING_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();

	void Init(AFYPTrainingGameMode* gmRef);

	void triggerMineBuild();

public:	

	AFYPTrainingGameMode* gamemodeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatManager")
	TSubclassOf<AActor> combatManager;
	
	ACombatManager* cmRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	TSubclassOf<AActor> mineClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	FVector mine1SpawnLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	FVector mine2SpawnLoc;

	AActor* mine1Spawned;
	AActor* mine2Spawned;

	AShipyard* shipyardRef;
private:

	FTimerHandle mineStatusCheckTimer;

	void checkMineStatus();
};
