// Written by Thomas Stevens, all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitManager.generated.h"

struct FConstructionData;
class AShipyard;
class AFYPTrainingGameMode;
class ACombatManager;
class AAIMasterControlManager;

UCLASS()
class FYPTRAINING_API AUnitManager : public AActor
{
	GENERATED_BODY()
	
public:	
	//Functions
	AUnitManager();

	void Init(AFYPTrainingGameMode* gmRef, AAIMasterControlManager* masterRef);

	void setShipyardPtr(AShipyard* shipyardRef);

	void upgradeTechLevel(FConstructionData* currentRowRef);

	//References
	ACombatManager* combatManagerRef;

	AShipyard* shipYardRef;

	AAIMasterControlManager* masterManagerRef;

	FConstructionData* currentRow;

	AFYPTrainingGameMode* gamemodeRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle dataTableRef;

	//Variables
	UPROPERTY(EditAnywhere, Category = "ArrayValues")
	TArray<FString> availableUnitsToBuild;

	UPROPERTY(EditAnywhere, Category = "ArrayValues")
	TArray<int> unitWeights;

private:
	//Variables
	int currentTechLevel;

	//Functions
	void getCombatManagerRef();

	void addNewUnitOptions();

	void startUnitBuildingOperation();

	int determineUnitToBuild();

	int findUnitWeightValue(FString rowName);

	FConstructionData* findCurrentRow(FString rowName);

	FTimerHandle unitConstructionTimer;
};
