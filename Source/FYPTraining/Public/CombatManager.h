// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

class AFYPTrainingGameMode;
class AResourceManager;

UCLASS()
class FYPTRAINING_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	//Functions
	ACombatManager();

	void taskCaptureMine();

	void Init(AFYPTrainingGameMode* gmRef, AResourceManager* rmRef, AActor* passedMine1, AActor* passedMine2, bool useLanchester);

	//Variables
	int shipsConstructedThisCycle = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* retreatLocationActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* playerShipyardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* passedMine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	bool usingLanchesterModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	TSubclassOf<AActor> corvetteSpawnRef;

private:
	//Functions
	void captureInitialMines();

	void taskOrderUnit(int passedOrderCode);

	void selectorCombatPredictionAlgorithm();

	void selectorCaptureMineOrder();

	bool LanchesterModelAlgorithm();

	bool BasicCombatPredictionAlgorithm();

	float calculateArmyStrength(bool calculateAIStrength);

	//Variables
	bool captureMine = false;

	bool shouldAttack;

	bool initStageComplete = false;

	TArray<AActor*> mineRefArray;

	//References
	FTimerHandle initDelay;

	FTimerHandle combatLoopTimer;

	AResourceManager* resourceManRef;

	AFYPTrainingGameMode* gamemodeRef;
};