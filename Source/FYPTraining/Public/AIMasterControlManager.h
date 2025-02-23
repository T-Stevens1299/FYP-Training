// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIMasterControlManager.generated.h"

class AFYPTrainingGameMode;
class AResourceManager;
class AUnitManager;

UCLASS()
class FYPTRAINING_API AAIMasterControlManager : public AActor
{
	GENERATED_BODY()
	
public:	
	//Functions
	AAIMasterControlManager();

	void Init(AFYPTrainingGameMode* gmRef, bool useDDS, bool useLanchester);

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	bool dynamicDifficultyScalingActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float baseSRate = 0.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float aimScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	float learningRate = 0.01;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIProperties")
	TSubclassOf<AResourceManager> rmRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIManagers")
	TSubclassOf<AActor> resourceManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIManagers")
	TSubclassOf<AActor> unitManager;

	AResourceManager* resourceManagerRef;

private:
	//Functions
	void dynamicDifficultyScalingCheck();

	float compareEconomy();

	float compareUnitPower();

	//References
	FTimerHandle ddsCheckTimer;

	AUnitManager* unitManagerRef;

	AFYPTrainingGameMode* gamemodeRef;
};