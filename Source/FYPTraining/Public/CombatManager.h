// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	ACombatManager();

protected:


public:	

	void captureInitialMines();

	void Init(AFYPTrainingGameMode* gmRef, AResourceManager* rmRef, AActor* passedMine1, AActor* passedMine2);

	void taskCaptureMine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* retreatLocationActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* playerShipyardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* passedMine;

	AActor* retreatPoint;

	AActor* attackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	bool usingLanchesterModel;

	bool captureMine = false;

	bool shouldAttack;

	bool initStageComplete = false;

	AFYPTrainingGameMode* gamemodeRef;

	AResourceManager* resourceManRef;

	FTimerHandle combatLoopTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	TSubclassOf<AActor> corvetteSpawnRef;

private:
	//Behaviour Tree Tasks
	void taskOrderUnit(int passedOrderCode);

	void selectorCombatPredictionAlgorithm();

	void selectorCaptureMineOrder();

	bool LanchesterModelAlgorithm();

	bool BasicCombatPredictionAlgorithm();

	float calculateArmyStrength(bool calculateAIStrength);

	TArray<AActor*> mineRefArray;

	FTimerHandle initDelay;
};
