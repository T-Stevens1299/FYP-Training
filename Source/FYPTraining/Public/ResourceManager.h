// Written by Thomas Stevens, all rights reserved

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
	//Functions
	AResourceManager();

	void Init(AFYPTrainingGameMode* gmRef, bool useLanchester);

	void triggerMineBuild();

	//Variables
	ACombatManager* cmRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatManager")
	TSubclassOf<AActor> combatManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	TSubclassOf<AActor> mineClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	FVector mine1SpawnLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MineProperties")
	FVector mine2SpawnLoc;

private:
	//References
	AActor* mine1Spawned;

	AActor* mine2Spawned;

	AShipyard* shipyardRef;

	AFYPTrainingGameMode* gamemodeRef;

	FTimerHandle mineStatusCheckTimer;

	//Functions
	void checkMineStatus();
};
