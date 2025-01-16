// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

class AFYPTrainingGameMode;

UCLASS()
class FYPTRAINING_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

protected:


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(AFYPTrainingGameMode* gmRef);

	void captureInitialMines(AFYPTrainingGameMode* gmRef, AActor* passedMine1, AActor* passedMine2);

	void taskCaptureMine();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* retreatLocationActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* playerShipyardRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	AActor* passedMine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManagerProperties")
	bool usingLanchesterModel;

	bool captureMine;

	bool shouldAttack;

	bool initStageComplete = false;

	AFYPTrainingGameMode* gamemodeRef;

private:
	//Behaviour Tree Tasks
	void taskOrderUnit(int passedOrderCode);

	void selectorCombatPredictionAlgorithm();

	void selectorCaptureMineOrder();

	bool LanchesterModelAlgorithm();

	bool BasicCombatPredictionAlgorithm();

	float calculateArmyStrength(bool calculateAIStrength);

};
