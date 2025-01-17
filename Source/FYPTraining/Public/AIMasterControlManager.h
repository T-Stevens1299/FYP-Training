// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIMasterControlManager.generated.h"

class AFYPTrainingGameMode;
class AResourceManager;
class AResearchManager;
class AUnitManager;

UCLASS()
class FYPTRAINING_API AAIMasterControlManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIMasterControlManager();

	void Init(AFYPTrainingGameMode* gmRef);



public:	
	AFYPTrainingGameMode* gamemodeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	TSubclassOf<AResourceManager> rmRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	bool dynamicDifficultyScalingActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float baseSRate = 0.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float aimScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float learningRate = 0.01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIManagers")
	TSubclassOf<AActor> resourceManager;

	AResourceManager* resourceManagerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIManagers")
	TSubclassOf<AActor> researchManager;

	AResearchManager* researchManagerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIManagers")
	TSubclassOf<AActor> unitManager;

	AUnitManager* unitManagerRef;

private:
	FTimerHandle ddsCheckTimer;

	void dynamicDifficultyScalingCheck();

	float compareEconomy();

	float compareUnitPower();

};
